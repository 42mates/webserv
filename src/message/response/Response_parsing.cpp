/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_parsing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:42:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/25 14:30:33 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::parseStartLine(string start_line)
{
	istringstream stream(start_line);
	string version, code, reason;

	if (!(stream >> version >> code)
		|| !(stream >> ws && getline(stream, reason)))
		throw ResponseException(Response("502"), "invalid start line format");	

	if (!( version != "HTTP/0.9" 
		|| version != "HTTP/1.0" 
		|| version != "HTTP/1.1"))
		throw ResponseException(Response("502"), "unsupported HTTP version format");
	
	if (_status_line.find(code) == _status_line.end())
		throw ResponseException(Response("502"), "unknown status code \"" + code + "\"");
	else if (_status_line[code] != reason)
		throw ResponseException(Response("502"), "invalid reason phrase \"" + reason + "\"");

	_code = code;
}

void Response::parseHeaderLine(string header_line)
{
	size_t pos = header_line.find(":");
	if (pos == 0 || pos == string::npos)
		throw ResponseException(Response("502"), "invalid header line format: \"" + header_line + "\"");

	string key = header_line.substr(0, pos);
	header_line.erase(0, pos + 1);
	while (header_line[0] == ' ')
		header_line.erase(0, 1);

	while (!header_line.empty()
		&& (header_line[header_line.size() - 1] == (' ' | '\t' | '\r' | '\n')))
		header_line.erase(header_line.size() - 1);

	if (header_line.empty())
		throw ResponseException(Response("502"), "empty header value");

	transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (_header.find(key) == _header.end())
		throw ResponseException(Response("502"), "unknown header key \"" + key + "\"");

	_header[key] = header_line;
}

void Response::parseHeader(string raw_response)
{
	size_t end = 0;
	string line;

	while ((end = raw_response.find("\n")) == 0)
		_start += end + 1;
	if (_start == string::npos)
		throw ResponseException(Response("502"), "empty Response");

	parseStartLine(raw_response.substr(_start, end - _start));
	_start = end + 1;

	while ((end = _start + raw_response.substr(_start).find("\n")) != _start)
	{
		line = raw_response.substr(_start, end - _start);
		if (line.empty())
			break;
		parseHeaderLine(line);
		_start = end + 1;
	}
	if (_start == string::npos)
		throw ResponseException(Response("502"), "no newline at the end of the headers");
	else
		_start += 1;
}

void Response::parseBody(string body)
{
	if (body.empty())
		return;

	if (!_header["content-length"].empty()
		&& body.size() != strtoul(_header["content-length"].c_str(), NULL, 10))
		throw ResponseException(Response("502"), "invalid content-length (expected: " + _header["content-length"] + ", got: " + itostr(body.size()) + ")");
	_body = body;
}

void Response::parseResponse(string raw_response)
{
	try
	{
		parseHeader(raw_response);
		if (_header["content-length"].empty() 
			&& !_header["transfer-encoding"].empty()
			&& _header["transfer-encoding"] != "chunked")
			throw ResponseException(Response("411"), "missing content-length header");
		parseBody(raw_response.substr(_start));
	}
	catch(const ResponseException& e)
	{
		std::cerr << "debug: parseResponse(): " << e.what() << endl;
		*this = e.getResponse();
	}
	catch(const exception& e)
	{
		std::cerr << "debug: parseResponse() (std exception): " << e.what() << endl;
	}
}
