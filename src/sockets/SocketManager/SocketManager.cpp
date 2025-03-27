/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/26 23:50:41 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/*
for one port having a vector of servers
get headeerValue with host for having domain and port
then find ServerConfig
then use function
if no associated ServerConfig
then use first ServerConfig in vector
*/

/**
 * @file SocketManager.cpp
 * @brief Implementation of the SocketManager class for managing server and client sockets.
 * 
 * This file contains the implementation of the SocketManager class, which is responsible for
 * creating, configuring, and managing server and client sockets. It includes methods for
 * creating sockets, setting socket options, binding, listening, and adding sockets to the poll
 * structure. The destructor ensures that all sockets are properly closed.
 */
 

//? Can there be multiple ServerConfig on one given port ?
/**
 * @brief Generates a server socket and its informations for each ports given as a parameter.
 */
SocketManager::SocketManager(const vector <ServerConfig*>* servers) : _poll_manager(*this)
{
	for (size_t i = 0; i < servers->size(); i++)
	{
		try
		{
			int port = servers->at(i)->port;
			_ports_info[port];
			storeAssociatedServers(port, servers);
			_ops.createSocket(servers->at(i)->host, port, _ports_info[port]);
			_ops.setReusability(_ports_info[port].server_fd);
			_ops.setOptions(_ports_info[port].server_fd);
			_ops.bindSocket(_ports_info[port], port);
			_ops.listenSocket(_ports_info[port]);
			storeSocket(port, _ports_info[port].server_fd, (POLLIN | POLLERR | POLLHUP), SERVER_SOCKET, NULL);
			// _ports_info[port].servers = servers;
		}
		catch(exception& e) { cout << e.what() << endl; }
	}
}

//!PULL BEFORE

/**
 * @brief Ensures that all client and server sockets are closed for each port 
 * managed by the SocketManager.
 */
SocketManager::~SocketManager(void)
{
	for (map<int, PortInfo>::iterator port_it = _ports_info.begin(); port_it != _ports_info.end(); port_it++)
	{
		vector<ClientInfo>&	a_client = port_it->second.clients; 
		for (size_t i = 0; i  < a_client.size(); i++)
		{
			if (a_client[i].client_fd != -1)
				close(a_client[i].client_fd);
		}
		if (port_it->second.server_fd != -1)
			close(port_it->second.server_fd);
	}
}
