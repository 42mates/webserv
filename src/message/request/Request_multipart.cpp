/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_multipart.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:41:50 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:14:58 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

typedef struct MultipartBody
{
	map<string, string>	headers;
} MultipartBody;

typedef struct MultipartData
{
	string				type;
	map<string, string>	settings;
	MultipartBody		body;
} MultipartData;

static void parseContentType(string header, MultipartData &data)
{
	istringstream iss(header);
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

static void parseMultipartHeader(string body, MultipartBody &file)
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

		file.headers[key] = value;
		if (key == "content-disposition")
		{
			size_t name_pos = value.find("name=\"");
			if (name_pos != string::npos)
			{
				size_t name_end = value.find("\"", name_pos + 6);
				file.headers["name"] = value.substr(name_pos + 6, name_end - name_pos - 6);
			}
			size_t filename_pos = value.find("filename=\"");
			if (filename_pos != string::npos)
			{
				size_t filename_end = value.find("\"", filename_pos + 10);
				file.headers["filename"] = value.substr(filename_pos + 10, filename_end - filename_pos - 10);
			}
		}

		pos = end_pos + 2; // Move past CRLF
	}
}

void Request::decodeMultipart(string &upload_dir, string &filename, fstream &outfile)
{
	MultipartData data;
	parseContentType(_header["content-type"], data);
	
	string body = getBodyString();
	string boundary = "--" + data.settings["boundary"];
	size_t boundary_len = boundary.size();
	size_t pos = body.find(boundary) + boundary_len;
	size_t next_pos = 0;

	if (pos != boundary_len)
		throw ResponseException(Response("400"), "invalid multipart body format: no initial boundary");
		
	MultipartBody file;
	
	while ((next_pos = body.find(string(CRLF) + boundary, pos)) != string::npos)
	{
		string entity = body.substr(pos + 2, next_pos - pos - 2); // + 2 = CRLF
		if (entity.empty())
			throw ResponseException(Response("400"), "empty multipart entity");

		size_t sep = entity.find(string(CRLF) + CRLF);
		if (sep == string::npos)
			throw ResponseException(Response("400"), "invalid multipart entity format");

		parseMultipartHeader(entity.substr(0, sep), file);

		if (file.headers.find("filename") != file.headers.end()
			&& upload_dir + "/" + file.headers["filename"] != filename)
		{
			filename = upload_dir + "/" + file.headers["filename"];
			getOutfile(filename, upload_dir, outfile);
		}
		else if (!outfile.is_open())
			getOutfile(filename, upload_dir, outfile);
		
		outfile << entity.substr(sep + 4);
		pos = next_pos + 2 + boundary_len;
		if (pos >= body.size())
			break; 
	}
}
