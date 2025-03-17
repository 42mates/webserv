/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/17 17:58:54 by mbecker          ###   ########.fr       */
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

void Request::parseHeader(string raw_request)
{
	size_t end = 0;
	string line;

	while ((end = raw_request.substr(_start).find(CRLF)) == 0)
		_start += end + 2;
	if (_start == string::npos)
		throw ResponseException(Response("400"), "empty request");

	parseStartLine(raw_request.substr(_start, end - _start));
	_start = end + 2;

	while ((end = _start + raw_request.substr(_start).find(CRLF)) != _start)
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

void Request::parseBody(string body)
{
	if (body.empty())
		return;

	if ( _header["transfer-encoding"] == "chunked")
		_body = decodeChunked(body);
	else if (!_header["transfer-encoding"].empty()
		&& _header["transfer-encoding"] != "identity")
		throw ResponseException(Response("501"), "transfer-encoding value \"" + _header["transfer-encoding"] + "\" not supported"); 
	else
	{
		if (!_header["content-length"].empty()
			&& body.size() != strtoul(_header["content-length"].c_str(), NULL, 10))
			throw ResponseException(Response("400"), "invalid content-length (expected: " + _header["content-length"] + ", got: " + itostr(body.size()) + ")");
		_body += body;
	}
}

void Request::parseRequest(string request_chunk)
{
	//cerr << (_parsingcalls++ == 0 ? "\n" : "");
	//cerr << BLUE << "parsing call [" << _parsingcalls << "]" << NC << endl;
	
	_raw_request += request_chunk;

	//TODO: check if _raw_request is too large
	//if (_raw_request.size() > _conf.max_body_size)
	//	throw ResponseException(Response("413"), "request body too large");
	//cout << "new size of _raw_request: " << _raw_request.size() << endl;

	setIsCompleteRequest();

	//if (_raw_request.substr(0, 10).find("GET") == string::npos) // debug: avoid printing GET requests
		//this->printRaw(); //! debug to see recv() results
	
	if (!_header_parsed && isCompleteHeader(_raw_request))
		parseHeader(_raw_request);

	if (_header_parsed && _header["expect"] == "100-continue")
	{
		throw ContinueException();
		_header["expect"] = "";
	}
	
	setIsCompleteRequest();
	if (_method != "GET" || _method != "HEAD")
	{
		if (_header["content-length"].empty() 
			&& !_header["transfer-encoding"].empty()
			&& _header["transfer-encoding"] != "chunked")
			throw ResponseException(Response("411"), "missing content-length header");

		if (_is_complete_request)
			parseBody(_raw_request.substr(_start));
	}
}
