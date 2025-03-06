/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:10:07 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/06 12:01:04 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::checkStartLine()
{
	if (_method.empty() || _uri.empty() || _version.empty())
		throw ResponseException(Response("400"), "missing required start line field");
	
	// check if method is allowed
	if (_method_handling.find(_method) == _method_handling.end())
		throw ResponseException(Response("405"), "unknown method");
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

string Request::getFilePath(const string &path)
{
	vector<string> paths;

	paths.push_back(path);
	for (size_t i = 0; i < _route_conf.index_file.size(); ++i)
		paths.push_back(path + "/" + _route_conf.index_file[i]);
	
	for (size_t i = 0; i < paths.size(); ++i)
	{
		struct stat buffer;
		if (stat(paths[i].c_str(), &buffer) == 0)
		{
			if (S_ISREG(buffer.st_mode) && access(paths[i].c_str(), R_OK) == 0)
				return paths[i];
		}
	}
	throw ResponseException(Response("404"), "getFilePath(): could not find file " + path);	
}

Response Request::handleRequest(ServerConfig &server_conf)
{
	Response response;

	// find the best matching route
	_server_conf = server_conf;
	_route_conf = getBestRoute(_server_conf, _uri);
	//printConfig(_server_conf);
	//printConfig(_route_conf);

	try
	{
		checkStartLine();
		checkHeader();
		_path = getFilePath(_route_conf.root + _uri);
		// check if method is authorized
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
