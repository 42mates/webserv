/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:10:07 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 14:42:22 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static bool isResourceAvailable(const std::string &uri)
{
	struct stat buffer;
	
	if (stat(uri.c_str(), &buffer) != 0)
		return false;

	// Check if the resource is a regular file or a directory and if it is readable
	if ((S_ISREG(buffer.st_mode) || S_ISDIR(buffer.st_mode))
		&& access(uri.c_str(), R_OK) == 0)
		return true;

	return false;
}

void Request::checkStartLine()
{
	if (_method.empty() || _uri.empty() || _version.empty())
		throw ResponseException(Response("400"), "missing required start line field");
	
	// check if method is allowed
	if (_method_handling.find(_method) == _method_handling.end())
		throw ResponseException(Response("405"), "unknown method");

	// check if resource is available and accessible
	if (!isResourceAvailable(_uri))
		throw ResponseException(Response("404"), "uri not found or not accessible");
}

void Request::checkHeader()
{
	if (_header["host"].empty())
		throw ResponseException(Response("400"), "missing required header field \"Host\"");
	if (!_header["expect"].empty() && _header["expect"] != "100-continue")
		throw ResponseException(Response("417"), "expect value not supported");

	//if (_header["content-length"].empty())
	//	throw ResponseException(Response("400"), "missing required header field \"Content-Length\"");
}


Response Request::handleRequest(ServerConfig &server_conf)
{
	Response response;

	// find the best matching route
	_server_conf = server_conf;
	_route_conf = getBestRoute(_server_conf, _uri);

	try
	{
		checkStartLine();
		checkHeader();
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleRequest(): " << e.what() << endl;
		return e.getResponse();
	}

	// handle the request
	response = (this->*_method_handling[_method])();

	return response;
}
