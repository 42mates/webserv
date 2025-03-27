/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/27 15:28:41 by mbecker          ###   ########.fr       */
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
 * managed by the SocketManager, as well as for each instance in the _poll_fds vector.
 */
SocketManager::~SocketManager(void)
{
	// Close each socket per port
	for (map<int, PortInfo>::iterator port_it = _ports_info.begin(); port_it != _ports_info.end(); port_it++)
	{
		vector<ClientInfo>&	a_client = port_it->second.clients; 
		for (size_t i = 0; i  < a_client.size(); i++)
		{
			if (a_client[i].client_fd != -1)
			{
				close(a_client[i].client_fd);
				a_client[i].client_fd = -1;
			}
		}
		if (port_it->second.server_fd != -1)
		{
			close(port_it->second.server_fd);
			port_it->second.server_fd = -1;
		}
	}

	// Close each socket in the _poll_fds vector
	for (size_t i = 0; i < _poll_fds.size(); i++)
	{
		if (_poll_fds[i].fd != -1)
			close(_poll_fds[i].fd);
	}
}
