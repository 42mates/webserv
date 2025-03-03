/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/03 14:38:20 by mbecker          ###   ########.fr       */
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
	if (!( version != "HTTP/0.9" 
		|| version != "HTTP/1.0" 
		|| version != "HTTP/1.1"))
		throw ResponseException(Response("505"), "unsupported HTTP version format");
	_version = version;
}

void Request::parseHeaderLine(string header_line)
{
	size_t pos = header_line.find(":");
	if (pos == 0 || pos == string::npos)
		throw ResponseException(Response("400"), "invalid header line format: \"" + header_line + "\"");

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
		throw ResponseException(Response("400"), "unknown header key \"" + key + "\"");

	_header[key] = header_line;
}

void Request::parseBody(string body)
{
	if (body.empty())
		return;

	if ( _header["transfer-encoding"] == "chunked")
		_body += decodeChunked(body);
	else if (!_header["transfer-encoding"].empty()
		&& _header["transfer-encoding"] != "identity")
		throw ResponseException(Response("501"), "transfer-encoding value \"" + _header["transfer-encoding"] + "\" not supported"); 
	else
	{
		if (!_header["content-length"].empty()
			&& body.size() != strtoul(_header["content-length"].c_str(), NULL, 10))
			throw ResponseException(Response("400"), "invalid content-length");
		_body += body;
	}
}

void Request::parseHeader(string raw_request)
{
	size_t end = 0;
	string line;

	while ((end = raw_request.substr(_start).find("\r\n")) == 0)
		_start += end + 2;
	if (_start == string::npos)
		throw ResponseException(Response("400"), "empty request");

	parseStartLine(raw_request.substr(_start, end - _start));
	_start = end + 2;

	while ((end = _start + raw_request.substr(_start).find("\r\n")) != _start)
	{
		line = raw_request.substr(_start, end - _start);
		if (line.empty())
			break;
		parseHeaderLine(line);
		_start = end + 2;
	}
	if (_start == string::npos)
		throw ResponseException(Response("400"), "no CRLF at the end of the headers");
	else
		_start += 2;
	_header_parsed = true;
}

bool Request::isCompleteHeader(string raw_request)
{
	size_t end = 0;
	string line;

	while ((end = raw_request.find("\r\n")) == 0)
		raw_request.erase(0, end + 2);
	if (raw_request.empty())
		return false;

	if (raw_request.find("\r\n\r\n") == string::npos)
		return false;
	return true;
}

void Request::parseRequest(string request_chunk)
{
	_raw_request += request_chunk;
	
	if (!_is_complete_request) 
		cout << "parseRequest called with incomplete request" << endl;

	cout << "\n------RAW REQUEST------" << endl; {
		size_t i = 0;
		for (; i < _raw_request.size(); ++i) 
		{
			if (_raw_request[i] == '\r')
				cout << RED << "\\r" << NC;
			else if (_raw_request[i] == '\n')
				cout << RED << "\\n" << NC << endl;
			else
				cout << _raw_request[i];
		}
		if (--i == _raw_request.size() - 1 && _raw_request[i] != '\n')
			cout << BRED << "%\n" << NC;}
	cout << "-----------------------" << endl;

	if (isCompleteHeader(_raw_request) && !_header_parsed)
		parseHeader(request_chunk);

	if (_header_parsed && _header["expect"] == "100-continue")
	{
		throw ContinueException();
		_header["expect"] = "";
	}

	if (_method != "GET" || _method != "HEAD")
	{
		//if (_header["content-length"].empty() 
		//	&& !_header["transfer-encoding"].empty()
		//	&& _header["transfer-encoding"] != "chunked")
		//	throw ResponseException(Response("411"), "missing content-length header");

		if (_is_complete_request)
			parseBody(_raw_request.substr(_start));
	}
}
