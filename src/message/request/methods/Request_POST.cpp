/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 15:08:22 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// namespace used like static for functions.

string parseURLEncoded(string body)
{
	istringstream iss(body);
	string result;

	try
	{
		vector< pair<string, string> > decoded = decodeURL(body);
		for (vector< pair<string, string> >::iterator it = decoded.begin(); it != decoded.end(); it++)
			result += it->first + "=" + it->second + "\n";
	}
	catch(const runtime_error& e)
	{
		throw ResponseException(Response("500"), e.what());
	}

	return result;
}

string getUniqueFilename(string &upload_dir)
{
	static size_t unique_id = 0; // Static variable to maintain ascending order of unique IDs
	string filename;
	struct stat buffer;
	do {
		filename = upload_dir + "/webserv_resource_" + itostr(unique_id++);
	} while (stat(filename.c_str(), &buffer) == 0); // Check if file exists
	return filename;
}


void Request::getOutfile(string &filename, string &upload_dir, fstream &file)
{
	struct stat info;
	if (stat(upload_dir.c_str(), &info) != 0)
		throw ResponseException(Response("500"), "upload directory \"" + upload_dir + "\" does not exist");
	if (!(info.st_mode & S_IFDIR))
		throw ResponseException(Response("500"), "\"" + upload_dir + "\" is not a directory");
	if (access(upload_dir.c_str(), W_OK | R_OK) != 0)
		throw ResponseException(Response("500"), "no read or write access to upload directory \"" + upload_dir + "\"");

	if (!upload_dir.empty() && upload_dir[upload_dir.size() - 1] == '/')
		upload_dir.erase(upload_dir.size() - 1);

	if (stat(filename.c_str(), &info) == 0)
		throw ResponseException(Response("409"), "file \"" + filename + "\" already exists");

	file.open(filename.c_str(), ios::out | ios::in | ios::binary | ios::trunc);
	if (!file.is_open() || !file.good())
		throw ResponseException(Response("500"), "failed to create file \"" + filename + "\": " + strerror(errno));

	// Check if the file was created successfully
	struct stat file_info;
	if (stat(filename.c_str(), &file_info) != 0)
		throw ResponseException(Response("500"), "failed to create file \"" + filename + "\": " + strerror(errno));

	// Check for write access
	if (access(filename.c_str(), W_OK | R_OK) != 0)
		throw ResponseException(Response("500"), "no read or write access to file \"" + filename + "\"");

	cout << "Created file: " << filename << endl;
}

void uploadStream(fstream &stream, fstream &file)
{
	
	// Get the size of the body stream
	stream.seekg(0, ios::end);
	size_t size = stream.tellg();
	stream.seekg(0, ios::beg);

	// Read the body stream into a buffer and write it to the file
	vector<char> buffer(size);
	stream.read(buffer.data(), size);
	file.write(buffer.data(), size);
}

Response Request::handlePOST()
{
	Response response;
	fstream outfile;

	try
	{
		if (getBodySize() == 0)
		{
			if (!_query.empty())
				_body_stream << parseURLEncoded(_query);
			//else
			//	return Response("405"); asked for subject
		}

		string upload_dir = _route_conf.upload_dir.empty() ? _route_conf.root : _route_conf.upload_dir;
		string filename = getUniqueFilename(upload_dir);

		if (getHeaderValue("content-type") == "multipart/form-data")
			decodeMultipart(upload_dir, filename, outfile);
		else
		{
			getOutfile(filename, upload_dir, outfile);
			uploadStream(_body_stream, outfile);
		}
	
		response = Response("200");
		string response_body = "POST request received successfully:\n\n";
		
		response.setBody(response_body + (getBodySize() > 2048 ? "[Body too large to display]\n" : getBodyString(outfile)));
	}
	catch(const ResponseException& e)
	{
		cerr <<  "handlePOST(): " << e.what() << endl;
		response = e.getResponse();
	}
	catch(const runtime_error& e)
	{
		cerr <<  "handlePOST(): " << e.what() << endl;
		response = Response("500");
	}
	if (outfile.is_open())
		outfile.close();

	return response;
}
