/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/05 15:57:14 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
	: _method(""), _uri(""), _version("")
{
	_header["expect"];             // Required - Used for `100-continue` before sending request body
	_header["host"];               // Required - Specifies the target host (mandatory in HTTP/1.1)
	_header["content-Length"];     // Required - Specifies the size of the request body

	// OPTIONAL HEADER FIELDS
	//_header["Accept"];             // Optional - Specifies preferred media types (e.g., text/html, application/json)
	//_header["Accept-Charset"];     // Optional - Specifies preferred character sets (e.g., UTF-8)
	//_header["Accept-Encoding"];    // Optional - Specifies supported content encodings (e.g., gzip, deflate)
	//_header["Accept-Language"];    // Optional - Specifies preferred languages for response (e.g., en-US, fr-FR)
	//_header["Authorization"];      // Optional - Contains credentials for HTTP authentication (e.g., Basic, Bearer)
	//_header["Range"];              // Optional - Used for partial content requests (e.g., video streaming)
	//_header["Referer"];            // Optional - Indicates the previous page (useful for logs/security)
	//_header["User-Agent"];         // Optional - Identifies the client (browser, bot, etc.), useful for logs
	//_header["From"];               // Ignored - Rarely used, contains client’s email address
	//_header["If-Match"];           // Ignored - Used for conditional requests based on ETag
	//_header["If-Modified-Since"];  // Ignored - Used for cache validation (not required in webserv)
	//_header["If-None-Match"];      // Ignored - Used to check if resource changed (ETag comparison)
	//_header["If-Range"];           // Ignored - Used for resuming downloads (Range requests)
	//_header["If-Unmodified-Since"];// Ignored - Conditional request based on modification date
	//_header["Max-Forwards"];       // Ignored - Used in TRACE and OPTIONS (not required in webserv)
	//_header["Proxy-Authorization"];// Ignored - Used for authentication with a proxy server
	//_header["TE"];                 // Ignored - Specifies transfer encodings (not required for webserv)

}

void Request::parseStartLine(string start_line)
{
	istringstream stream(start_line);
	string method, uri, version;

	if (!(stream >> method >> uri >> version))
		throw runtime_error("debug: invalid start line format");

	_method = method;
	_uri = uri;
	if (version.find("HTTP/") != 0)
		throw runtime_error("debug: invalid HTTP version format");
	_version = version;
}

void Request::parseHeaderLine(string header_line)
{
	size_t pos = header_line.find(":");
	if (pos == string::npos)
		throw runtime_error("debug: 400 Bad Request (invalid header line format)");
	
	string key = header_line.substr(0, pos);
	header_line.erase(0, pos + 1);
	while (header_line[0] == ' ')
		header_line.erase(0, 1);
	
	while (!header_line.empty()
		&& (header_line[header_line.size() - 1] == (' ' | '\t' | '\r' | '\n')))
		header_line.erase(header_line.size() - 1);

	if (header_line.empty())
		throw runtime_error("debug: 400 Bad Request (empty header value)");
	
	transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (_header.find(key) == _header.end())
		throw runtime_error(string("debug: ") + key + ": 400 Bad Request (invalid header key)");

	_header[key] = header_line;
}

void Request::parseBody(string body)
{
	if (_header["content-length"].empty())
		throw runtime_error("debug: 400 Bad Request (missing content-length header)");
	
	if ( _header["transfer-encoding"] == "chunked")
		_body = decodeChunked(body);
	else if (!_header["transfer-encoding"].empty()
		&& _header["transfer-encoding"] != "identity")
		throw runtime_error("debug: 501 Not Implemented (transfer-encoding value not supported)"); 
	else
	{
		if (body.size() != strtoul(_header["content-length"].c_str(), NULL, 10))
			throw runtime_error("debug: 400 Bad Request (invalid content-length)");
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
		throw runtime_error("debug: 400 Bad Request (empty request)");

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
		throw runtime_error("debug: 400 Bad Request (no CRLF at the end of the headers)");
	raw_request.erase(0, 2);
	if (!raw_request.empty())
		parseBody(raw_request);
}


void Request::test()
{
	string input;
	ifstream file("test.txt");
	for (string tmp; getline(file, tmp);)
		input += tmp + "\r\n";
	file.close();
	

	parseRequest(input);

	cout << "Method:  " << _method << endl;
	cout << "URI:     " << _uri << endl;
	cout << "Version: " << _version << endl;
	cout << "Header:  " << endl;
	for (map<string, string>::iterator it = _header.begin(); it != _header.end(); it++)
	{
		if (!it->second.empty())	
			cout << "\t" << it->first << ": " << it->second << endl;
	}
	cout << "Body: " << _body << endl;
}