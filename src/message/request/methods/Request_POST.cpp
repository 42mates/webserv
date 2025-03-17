/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/17 15:27:12 by mbecker          ###   ########.fr       */
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
		map<string, string> decoded = decodeURL(body);
		for (map<string, string>::iterator it = decoded.begin(); it != decoded.end(); it++)
		{
			POSTBody post_body;
			post_body.content = it->first + "=" + it->second;
			data.bodies.push_back(post_body);
		}
	}
	catch(const runtime_error& e)
	{
		throw ResponseException(Response("400"), e.what());
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
		{
			response = Response("400");
			response.setBody("");
			return response;
		}
			//cout << "debug: Empty body in PUT. what should I do with it ?" << endl; //? idk what to do with it yet
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
	}
	catch(const runtime_error& e)
	{
		cerr << "debug: handlePOST(): " << e.what() << endl;
	}	


	response = Response("200");
	//response = handleGET(); // for now, just to test

	string response_body = "POST request received successfully\n\n";
	for (size_t i = 0; i < data.bodies.size(); i++)
		response_body += "[" + itostr(i) + "] - \"" + data.bodies[i].content + "\"\n";

	response.setBody(response_body);

	return response;
}

// TEST ~!@#$%^&*()_+`1234567890-={}|[]\:";'<>?,./
