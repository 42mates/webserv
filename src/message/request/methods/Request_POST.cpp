/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/12 19:42:11 by mbecker          ###   ########.fr       */
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
		string				type;     // Content-Type
		map<string, string>	settings; // Content-Type settings (boundary, charset, etc.)
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

static void parseMultipartHeader(string body, POSTData &data)
{
	size_t pos = 0;
	size_t end = 0;
	string line;
	string boundary = "--" + data.settings["boundary"];

	body = body.substr(0, body.find(string(CRLF) + CRLF));

	while ((end = body.substr(pos).find(CRLF)) != string::npos)
	{
		line = body.substr(pos, end);
		if (line.empty())
			break;
		size_t sep = line.find(":");
		if (sep == string::npos)
			throw ResponseException(Response("400"), "invalid header line format");
		string key = line.substr(0, sep);
		string value = line.substr(sep + 1);
		data.bodies.back().headers[key] = value;
		pos += end + 2;
	}
}

static void parseMultipartBodies(string body, POSTData &data)
{
	string boundary = string(CRLF) + "--" + data.settings["boundary"];
	size_t pos = body.find(string(CRLF) + CRLF) + 4;
	size_t next_pos = 0;
	size_t boundary_len = boundary.size();
	size_t content_len = body.size();

	while (pos < content_len)
	{
		next_pos = body.find(boundary, pos);
		if (next_pos == string::npos)
			next_pos = content_len;
		POSTBody post_body;
		post_body.content = body.substr(pos, next_pos - pos);
		if (post_body.content == string("--") + CRLF)
			break;
		data.bodies.push_back(post_body);
		pos = next_pos + boundary_len;
	}

}

void percentDecode(string &str)
{
	//string base = "0123456789ABCDEF";
	size_t pos = 0;

	while ((pos = str.find('+', pos)) != string::npos)
		str.replace(pos, 1, 1, ' ');
	pos = 0;

	while ((pos = str.find('%', pos)) != string::npos)
	{
		if (pos + 2 >= str.size())
			throw ResponseException(Response("400"), "invalid percent encoding");
		
		string tmp = str.substr(pos + 1, 2);
		char c = (char)strtol(tmp.c_str(), NULL, 16);
		str.replace(pos, 3, 1, c);
	}
}

static void parseURLEncoded(string body, POSTData &data)
{
	
	istringstream iss(body);
	string field;
	
	while (getline(iss, field, '&'))
	{
		size_t pos = field.find('=');
		if (pos == string::npos)
			throw ResponseException(Response("400"), "invalid url-encoded field");
			
		percentDecode(field);
		string key = field.substr(0, pos);
		string value = field.substr(pos + 1);
			
		POSTBody post_body;
		post_body.content = key + "=" + value;
		data.bodies.push_back(post_body);
	}
}


Response Request::handlePOST()
{
	Response response;
	POSTData data;

	parseContentType(_header["content-type"], data);

	if (_body.empty())
		; //? idk what to do with it yet
	else if (data.type == "multipart/form-data")
	{
		parseMultipartHeader(_body, data);
		parseMultipartBodies(_body, data);
	}
	else if (data.type == "application/x-www-form-urlencoded")
	{
		parseURLEncoded(_body, data);
	}
	else
		throw ResponseException(Response("400"), "unsupported content-type");


	//print bodies debug
	for (size_t i = 0; i < data.bodies.size(); i++)
	{
		POSTBody body = data.bodies[i];
		cout << BMAGENTA << "Body " << i << ":\n" << MAGENTA;
		cout << body.content << BMAGENTA << "%" << endl;
		cout << NC;
	}
	cout << "Body parsed" << endl;


	response = Response("200");
	//response = handleGET(); // for now, just to test

	//temporary response 
	try { response.setBody(getFile("tools/website/upload/success.html")); }
	catch(const std::exception& e) { std::cerr << "debug: handlePOST(): " << e.what() << '\n'; }
	

	return response;
}

// TEST ~!@#$%^&*()_+`1234567890-={}|[]\:";'<>?,./
// TEST ~!@#$E&*()_+`1234567890-={}|[]\:";'<>?,./