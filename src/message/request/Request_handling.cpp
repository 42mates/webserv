/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:10:07 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/11 18:11:06 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Response Request::handleGet()
{
	return Response();
}

Response Request::handlePost()
{
	return Response();
}

Response Request::handleDelete()
{
	return Response();
}

static bool isResourceAvailable(const std::string &uri)
{
	struct stat buffer;
	
	if (stat(uri.c_str(), &buffer) != 0)
		return false;

	// Check if the resource is a regular file or a directory
	if (S_ISREG(buffer.st_mode) || S_ISDIR(buffer.st_mode))
		return true;

	return false;
}

Response Request::handleRequest(ServerConfig &server_conf)
{
	Response response;

	// find the best matching route
	_server_conf = server_conf;
	_route_conf = getBestRoute(_server_conf, _uri);

	// check if method is allowed
	if (_method_handling.find(_method) == _method_handling.end())
		return Response("405");

	// check if resource is available and accessible
	if (!isResourceAvailable(_uri))
		return Response("404");
	
	// handle the request
	response = (this->*_method_handling[_method])();
	

	return response;
}
