/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 14:24:11 by mbecker          ###   ########.fr       */
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

static void parseContentType(string ct_header_val, POSTData &data)
{
	istringstream iss(ct_header_val);
	string tmp; 
	
	getline(iss, tmp, ' ');
	if (tmp.empty())
		return;
	if (tmp[tmp.size() - 1] == ';')
		tmp.erase(tmp.size() - 1);
	data.type = tmp;

	while (getline(iss, tmp))
	{
		size_t pos = tmp.find("=");
		if (pos == string::npos)
			throw ResponseException(Response("400"), "invalid content-type header value");
		string key = tmp.substr(0, pos);
		string value = tmp.substr(pos + 1);
		data.settings[key] = value;
	}
}

static void parseMultipartHeader(string body, POSTBody &part)
{
	size_t pos = 0;
	size_t end_pos = 0;

	while ((end_pos = body.find("\r\n", pos)) != string::npos)
	{
		string line = body.substr(pos, end_pos - pos);
		if (line.empty())
			break;

		size_t sep_pos = line.find(": ");
		if (sep_pos == string::npos)
			throw ResponseException(Response("400"), "invalid multipart header format");

		string key = line.substr(0, sep_pos);
		transform(key.begin(), key.end(), key.begin(), ::tolower);
		string value = line.substr(sep_pos + 2);

		part.headers[key] = value;
		if (key == "content-disposition")
		{
			size_t name_pos = value.find("name=\"");
			size_t filename_pos = value.find("filename=\"");
			if (name_pos != string::npos)
			{
				size_t name_end = value.find("\"", name_pos + 6);
				part.headers["name"] = value.substr(name_pos + 6, name_end - name_pos - 6);
			}
			if (filename_pos != string::npos)
			{
				size_t filename_end = value.find("\"", filename_pos + 10);
				part.headers["filename"] = value.substr(filename_pos + 10, filename_end - filename_pos - 10);
			}
		}

		pos = end_pos + 2; // Move past CRLF
	}
}

static void parseMultipart(string body, POSTData &data)
{
	string boundary = "--" + data.settings["boundary"];
	size_t boundary_len = boundary.size();
	size_t pos = body.find(boundary) + boundary_len;
	size_t next_pos = 0;

	if (pos != boundary_len)
		throw ResponseException(Response("400"), "invalid multipart body format: no initial boundary");

	while ((next_pos = body.find(string(CRLF) + boundary, pos)) != string::npos)
	{
		string entity = body.substr(pos + 2, next_pos - pos - 2); // + 2 = CRLF
		if (entity.empty())
			throw runtime_error("empty multipart entity");

		POSTBody part;
		size_t sep = entity.find(string(CRLF) + CRLF);
		if (sep == string::npos)
			throw ResponseException(Response("400"), "invalid multipart entity format");

		parseMultipartHeader(entity.substr(0, sep), part);
		part.content = entity.substr(sep + 4);
		data.bodies.push_back(part);
		pos = next_pos + 2 + boundary_len;
	}
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
		throw ResponseException(Response("404"), "upload directory \"" + upload_dir + "\" does not exist");
	if (!(info.st_mode & S_IFDIR))
		throw ResponseException(Response("404"), "\"" + upload_dir + "\" is not a directory");
	if (access(upload_dir.c_str(), W_OK) != 0)
		throw ResponseException(Response("403"), "no write access to upload directory \"" + upload_dir + "\"");

	if (!upload_dir.empty() && upload_dir[upload_dir.size() - 1] == '/')
		upload_dir.erase(upload_dir.size() - 1);

	for (size_t i = 0; i < data.bodies.size(); i++)
	{
		if (data.bodies[i].headers.find("filename") != data.bodies[i].headers.end())
		{
			string filename = upload_dir + "/" + data.bodies[i].headers["filename"];
			ofstream file(filename.c_str(), ios::out | ios::binary | ios::app);
			if (!file.is_open() || !file.good())
				throw ResponseException(Response("403"), "failed to open file \"" + filename + "\" for writing");

			// Check if the file was created successfully
			struct stat file_info;
			if (stat(filename.c_str(), &file_info) != 0)
				throw ResponseException(Response("403"), "failed to create file \"" + filename + "\"");

			// Check for write access
			if (access(filename.c_str(), W_OK) != 0)
				throw ResponseException(Response("403"), "no write access to file \"" + filename + "\"");
			file << data.bodies[i].content;
			file.close();
		}
	}
}

Response Request::handlePOST()
{
	Response response;
	POSTData data;

	parseContentType(_header["content-type"], data);

	try
	{
		if (_body.empty()) 
			return Response("405");
		else if (data.type == "multipart/form-data")
			parseMultipart(_body, data);
		else if (data.type == "application/x-www-form-urlencoded")
			parseURLEncoded(_body, data);
		else if (data.type == "text/plain")
		{
			POSTBody body;
			body.content = _body;
			data.bodies.push_back(body);
		}
		else if (!data.type.empty())
			throw ResponseException(Response("400"), "unsupported content-type");

		cout << "route conf upload dir: " << _route_conf.upload_dir << endl;
		cout << "route conf root: " << _route_conf.root << endl;
		string upload_dir = _route_conf.upload_dir.empty() ? _route_conf.root : _route_conf.upload_dir;
		uploadPOST(data, upload_dir);
	}
	catch(const runtime_error& e)
	{
		cerr << "debug: handlePOST(): " << e.what() << endl;
	}


	

	response = Response("200");

	string response_body = "POST request received successfully:\n\n";
	for (size_t i = 0; i < data.bodies.size(); i++)
		response_body += data.bodies[i].content;
	//for (size_t i = 0; i < data.bodies.size(); i++)
	//	response_body += "[" + itostr(i) + "] - \"" + data.bodies[i].content + "\"\n";

	response.setBody(response_body);

	return response;
}

// TEST ~!@#$%^&*()_+`1234567890-={}|[]\:";'<>?,./
