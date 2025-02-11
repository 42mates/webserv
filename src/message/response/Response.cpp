/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/11 14:52:28 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::initStatusLine()
{
	_status_line["100"] = "Continue";
	_status_line["101"] = "Switching Protocols";
	_status_line["200"] = "OK";
	_status_line["201"] = "Created";
	_status_line["202"] = "Accepted";
	_status_line["203"] = "Non-Authoritative Information";
	_status_line["204"] = "No Content";
	_status_line["205"] = "Reset Content";
	_status_line["206"] = "Partial Content";
	_status_line["300"] = "Multiple Choices";
	_status_line["301"] = "Moved Permanently";
	_status_line["302"] = "Found";
	_status_line["303"] = "See Other";
	_status_line["304"] = "Not Modified";
	_status_line["305"] = "Use Proxy";
	_status_line["307"] = "Temporary Redirect";
	_status_line["400"] = "Bad Request";
	_status_line["401"] = "Unauthorized";
	_status_line["402"] = "Payment Required";
	_status_line["403"] = "Forbidden";
	_status_line["404"] = "Not Found";
	_status_line["405"] = "Method Not Allowed";
	_status_line["406"] = "Not Acceptable";
	_status_line["407"] = "Proxy Authentication Required";
	_status_line["408"] = "Request Time-out";
	_status_line["409"] = "Conflict";
	_status_line["410"] = "Gone";
	_status_line["411"] = "Length Required";
	_status_line["412"] = "Precondition Failed";
	_status_line["413"] = "Request Entity Too Large";
	_status_line["414"] = "Request-URI Too Large";
	_status_line["415"] = "Unsupported Media Type";
	_status_line["416"] = "Requested range not satisfiable";
	_status_line["417"] = "Expectation Failed";
	_status_line["500"] = "Internal Server Error";
	_status_line["501"] = "Not Implemented";
	_status_line["502"] = "Bad Gateway";
	_status_line["503"] = "Service Unavailable";
	_status_line["504"] = "Gateway Time-out";
	_status_line["505"] = "HTTP Version not supported";
}

Response::Response() 
	: _status("DEFAULT") 
{
	initStatusLine();
	if (_status_line.find(_status) == _status_line.end())
		throw invalid_argument(string("debug: Response constructor with arg ") + _status + ": invalid status code");
	setErrorBody();
}

Response::Response(string status) 
	: _status(status) 
{
	initStatusLine();
	if (_status_line.find(_status) == _status_line.end())
		throw invalid_argument(string("debug: Response constructor with arg ") + _status + ": invalid status code");
	setErrorBody();
}

Response::Response(const Response &other) 
	: _status(other._status), _header(other._header), _body(other._body), _debug(other._debug) 
{
	initStatusLine();
	if (_status_line.find(_status) == _status_line.end())
		throw invalid_argument(string("debug: Response constructor with arg ") + _status + ": invalid status code");
	setErrorBody();
}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_status = other._status;
		_header = other._header;
		_body = other._body;
		_debug = other._debug;
	}
	return *this;
}

Response::~Response()
{}

void Response::setErrorBody()
{
	_body = string("<html>\r\n") 
		+ "<head><title>" + _status + " " + _status_line[_status] + "</title></head>\r\n"
		+ "<body>\r\n"
		+ "<center><h1>" + _status + " " + _status_line[_status] + "</h1></center>\r\n"
		+ "<hr><center>webserv/1.0</center>\r\n"
		+ "</body>\r\n"
		+ "</html>\r\n";
}

void Response::setHeader(string header)
{
	_header = header;
}

void Response::setBody(string body)
{
	_body = body;
}

void Response::setDebug(string debug)
{
	_debug = debug;
}

string Response::getResponse()
{
	return _status + "\n" + _header + "\n" + _body;
}

string Response::getStatus()
{
	return _status;
}

string Response::getReason()
{
	return _status_line[_status];
}

string Response::getHeader()
{
	return _header;
}

string Response::getBody()
{
	return _body;
}

string Response::getDebug()
{
	return _debug;
}

void Response::print()
{
	cout << "Status: " << _status << endl;
	cout << "Header: " << _header << endl;
	cout << "Body: " << _body << endl;
	cout << "Debug: " << _debug << endl;
}
