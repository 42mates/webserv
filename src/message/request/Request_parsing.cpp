/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/13 17:43:29 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::parseStartLine(string start_line)
{
	istringstream stream(start_line);
	string method, uri, version;

	if (!(stream >> method >> uri >> version))
		throw ResponseException(Response("400"), "invalid start line format");

	_method = method;
	_uri = uri;
	if (version.find("HTTP/") != 0)
		throw ResponseException(Response("400"), "invalid HTTP version format");
	_version = version;
}

void Request::parseHeaderLine(string header_line)
{
	size_t pos = header_line.find(":");
	if (pos == string::npos)
		throw ResponseException(Response("400"), "invalid header line format");

	string key = header_line.substr(0, pos);
	header_line.erase(0, pos + 1);
	while (header_line[0] == ' ')
		header_line.erase(0, 1);

	while (!header_line.empty()
		&& (header_line[header_line.size() - 1] == (' ' | '\t' | '\r' | '\n')))
		header_line.erase(header_line.size() - 1);

	if (header_line.empty())
		throw ResponseException(Response("400"), "empty header value");

	transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (_header.find(key) == _header.end())
		throw ResponseException(Response("400"), "invalid header key \"" + key + "\"");

	_header[key] = header_line;
}

void Request::parseBody(string body)
{
	if (body.empty())
		return;

	if ( _header["transfer-encoding"] == "chunked")
		_body = decodeChunked(body);
	else if (!_header["transfer-encoding"].empty()
		&& _header["transfer-encoding"] != "identity")
		throw ResponseException(Response("501"), "transfer-encoding value not supported)"); 
	else
	{
		if (body.size() != strtoul(_header["content-length"].c_str(), NULL, 10))
			throw ResponseException(Response("400"), "invalid content-length");
		_body = body;
	}
}

void Request::parseRequest(string raw_request)
{
	size_t pos = 0;
	string line;

	while ((pos = raw_request.find("\r\n")) == 0)
		raw_request.erase(0, 2);
	if (raw_request.empty())
		throw ResponseException(Response("400"), "empty request");

	parseStartLine(raw_request.substr(0, pos));
	raw_request.erase(0, pos + 2);

	while ((pos = raw_request.find("\r\n")) != 0)
	{
		line = raw_request.substr(0, pos);
		if (line.empty())
			break;
		parseHeaderLine(line);
		raw_request.erase(0, pos + 2);
	}
	if (pos == string::npos)
		throw ResponseException(Response("400"), "no CRLF at the end of the headers");
	raw_request.erase(0, 2);

	if (_method != "GET" || _method != "HEAD")
	{
		if (_header["content-length"].empty() && _header["transfer-encoding"] != "chunked")
			throw ResponseException(Response("411"), "missing content-length header");
		parseBody(raw_request);
	}
}
