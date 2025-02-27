/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager_getters.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:34:48 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/18 18:30:53 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


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
 * @brief Retrieves the map of all port information.
 * 
 *  Returns a pointer to the map that contains information about all the ports
 *  managed by the `SocketManager`. Each entry in the map corresponds to a port number and its
 *  associated `PortInfo` structure.
 * 
 * @return A pointer to the map of port information.
 */
map<int, PortInfo>*	SocketManager::getPortsInfo() { return &_ports_info; }

/**
 * @brief Retrieves information about a specific port.
 * 
 * Returns a pointer to the `PortInfo` structure associated with the specified port.
 * If the port is not found in the map, it prints an error message and returns `NULL`.
 * 
 * @param port The port number for which information is requested.
 * @return A pointer to the `PortInfo` structure for the specified port, or `NULL` if the port is not found.
 */
PortInfo*	SocketManager::getPortInfo(int port)
{
	if (_ports_info.find(port) == _ports_info.end())
	{
		cerr << "port informations at port [" << port << "] not found" << endl;
		return NULL;
	}
	return &_ports_info.at(port);
}

/**
 * @brief Retrieves information about clients connected to a specific port.
 * 
 * Returns a pointer to the vector of `ClientInfo` structures associated with the specified port.
 * If the port is not found in the map, it prints an error message and returns `NULL`.
 * 
 * @param port The port number for which client information is requested.
 * @return A pointer to the vector of `ClientInfo` structures for the specified port, or `NULL` if the port is not found.
 */
vector<ClientInfo>*	SocketManager::getClientsInfo(int port)
{
	if (_ports_info.find(port) == _ports_info.end())
	{
		cerr << "port informations at port [" << port << "] not found" << endl;
		return NULL;
	}
	return &(_ports_info.at(port).clients);
}

/**
 * @brief Retrieves the vector of poll file descriptors.
 * 
 * Returns a pointer to the vector that contains the poll file descriptors
 * managed by the `SocketManager`. Each entry in the vector corresponds to a
 * file descriptor being monitored for events.
 * 
 * @return A pointer to the vector of poll file descriptors.
 */
vector<pollfd>*	SocketManager::getPollFds() { return &_poll_fds; }

/**
 * @brief Retrieves the map of socket poll information.
 * 
 * Returns a pointer to the map that contains information about the sockets
 * being polled by the `SocketManager`. Each entry in the map corresponds to a
 * socket file descriptor and its associated `SocketPollInfo` structure.
 * 
 * @return A pointer to the map of socket poll information.
 */
map<t_sockfd, SocketPollInfo>*	SocketManager::getSocketToPoll() { return &_socket_to_poll;	}
