/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:02:23 by sokaraku          #+#    #+#             */
/*   Updated: 2025/04/02 12:31:39 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include  "SocketManager.hpp"


bool	SocketPollManager::isMessageEnd(t_sockfd socket_fd, __int8_t type)
{
	if (type == T_REQUEST)
	{
		map<t_sockfd, Request*>::iterator it = _socket_to_request.find(socket_fd);
		if (it == _socket_to_request.end())
			return false;
		if (it->second->getIsCompleteRequest() == true)
			return true;
		return false;
	}
	map<t_sockfd, infoResponse>::iterator it = _socket_to_response.find(socket_fd);
	if (it == _socket_to_response.end())
		return false;
	if (it->second.fully_sent == true)
		return true;
	return false;

}

/**
 * @brief Finds the best matching server configuration for a given request.
 * 
 * This function searches through a list of server configurations to find the one
 * that matches the "host" and "port" specified in the request's headers. If no match
 * is found, it returns a default value or throws an exception depending on the operation.
 * 
 * @param servers A vector of ServerConfig objects representing the available servers.
 * @param request The Request object containing the client's request data.
 * @param send_operation A boolean indicating whether the operation involves sending data.
 *                       If true and the "host" header is missing, an exception is thrown.
 * 
 * @return The index of the best matching server in the servers vector. Returns -1 if no
 *         match is found and send_operation is false. Returns 0 if no match is found
 *         and send_operation is true but the "host" header is provided.
 * 
 * @throws ResponseException If send_operation is true and the "host" header is missing
 *                           or empty, a ResponseException with a "400 Bad Request" response
 *                           is thrown.
 */
ssize_t	findBestServer(vector<ServerConfig>& servers, Request& request, bool send_operation)
{
	string	infos = request.getHeaderValue("host");
	size_t	index = infos.find(":");
	string	host, port;

	if (index == string::npos)
		index = 0;
	if (infos.empty())
	{
		if (send_operation == true)
			throw ResponseException(Response("400"), "Bad request: Host not provided");
		else
			return -1;
	}
	host = infos.substr(0, index);
	port = infos.substr(index + 1);
	for (size_t i = 0; i < servers.size(); i++)
	{
		for (size_t j = 0; j < servers[i].server_names.size(); j++)
		{
			if (servers[i].server_names[j] == host && servers[i].port == atoi(port.c_str()))
				return i;
		}
	}
	return 0;
}