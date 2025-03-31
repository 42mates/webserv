/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/31 15:16:09 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// namespace used like static for functions.
namespace {
	typedef struct POSTBody
	{
		map<string, string>	headers;
		string				content;
	} POSTBody;

	typedef struct POSTData 
	{
		string				type;     // whole request Content-Type
		map<string, string>	settings; // whole request Content-Type settings (boundary, charset, etc.)
		vector<POSTBody>	bodies;
	} POSTData;
}


void parseURLEncoded(string body, POSTData &data)
{
	istringstream iss(body);
	string field;
	
	try
	{
		vector< pair<string, string> > decoded = decodeURL(body);
		for (vector< pair<string, string> >::iterator it = decoded.begin(); it != decoded.end(); it++)
		{
			POSTBody post_body;
			post_body.content = it->first + "=" + it->second;
			data.bodies.push_back(post_body);
		}
	}
	catch(const runtime_error& e)
	{
		throw ResponseException(Response("500"), e.what());
	}
}

void uploadPOST(POSTData &data, string upload_dir)
{
	struct stat info;
	if (stat(upload_dir.c_str(), &info) != 0)
		throw ResponseException(Response("500"), "upload directory \"" + upload_dir + "\" does not exist");
	if (!(info.st_mode & S_IFDIR))
		throw ResponseException(Response("500"), "\"" + upload_dir + "\" is not a directory");
	if (access(upload_dir.c_str(), W_OK) != 0)
		throw ResponseException(Response("500"), "no write access to upload directory \"" + upload_dir + "\"");

	if (!upload_dir.empty() && upload_dir[upload_dir.size() - 1] == '/')
		upload_dir.erase(upload_dir.size() - 1);

	for (size_t i = 0; i < data.bodies.size(); i++)
	{
		if (data.bodies[i].headers.find("filename") != data.bodies[i].headers.end())
		{
			string filename = upload_dir + "/" + data.bodies[i].headers["filename"];
			ofstream file(filename.c_str(), ios::out | ios::binary | ios::app);
			if (!file.is_open() || !file.good())
				throw ResponseException(Response("500"), "failed to open file \"" + filename + "\" for writing");

			// Check if the file was created successfully
			struct stat file_info;
			if (stat(filename.c_str(), &file_info) != 0)
				throw ResponseException(Response("500"), "failed to create file \"" + filename + "\"");

			// Check for write access
			if (access(filename.c_str(), W_OK) != 0)
				throw ResponseException(Response("500"), "no write access to file \"" + filename + "\"");
			file << data.bodies[i].content;
			file.close();
		}
	}
}

void uploadPOST(fstream &body_stream, string upload_dir)
{
	struct stat info;
	if (stat(upload_dir.c_str(), &info) != 0)
		throw ResponseException(Response("500"), "upload directory \"" + upload_dir + "\" does not exist");
	if (!(info.st_mode & S_IFDIR))
		throw ResponseException(Response("500"), "\"" + upload_dir + "\" is not a directory");
	if (access(upload_dir.c_str(), W_OK) != 0)
		throw ResponseException(Response("500"), "no write access to upload directory \"" + upload_dir + "\"");

	if (!upload_dir.empty() && upload_dir[upload_dir.size() - 1] == '/')
		upload_dir.erase(upload_dir.size() - 1);

	static size_t unique_id = 0; // Static variable to maintain ascending order of unique IDs
	string filename = upload_dir + "/webserv_resource_" + itostr(unique_id++);
	ofstream outfile(filename.c_str(), ios::out | ios::binary | ios::app);
	if (!outfile.is_open() || !outfile.good())
		throw ResponseException(Response("500"), "failed to open outfile \"" + filename + "\" for writing");

	body_stream.seekg(0, ios::end);
	size_t size = body_stream.tellg();
	body_stream.seekg(0, ios::beg);

	vector<char> buffer(size);
	body_stream.read(buffer.data(), size);
	outfile.write(buffer.data(), size);
	outfile.close();
}

Response Request::handlePOST()
{
	Response response;
	POSTData data;

	cerr << "handlePOST() body: \"" << _body << "\"" << endl;
	cerr << "handlePOST() body_size: " << getBodySize() << endl;

	try
	{
		string upload_dir = _route_conf.upload_dir.empty() ? _route_conf.root : _route_conf.upload_dir;

		if (getBodySize() == 0)
			return Response("405");
		else if (_body.empty())
			uploadPOST(_body_stream, _route_conf.upload_dir);
		//else if (data.type == "application/x-www-form-urlencoded")
		//	parseURLEncoded(_body, data);
		else if (data.type == "text/plain" || data.type == "plain/text")
		{
			POSTBody body;
			body.content = _body;
			data.bodies.push_back(body);
		}
		else if (!data.type.empty())
			throw ResponseException(Response("400"), "unsupported content-type");

		if (!_body.empty())
			uploadPOST(data, upload_dir);
	}
	catch(const runtime_error& e)
	{
		cerr <<  "handlePOST(): " << e.what() << endl;
	}

	response = Response("200");

	string response_body = "POST request received successfully:\n\n";
	for (size_t i = 0; i < data.bodies.size(); i++)
		response_body += data.bodies[i].content;
	if (_body.empty())
		response_body = "[Body too large to display]\n";
	response.setBody(response_body);

	return response;
}

// TEST ~!@#$%^&*()_+`1234567890-={}|[]\:";'<>?,./
