/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_getters.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:34:48 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/04 17:51:11 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

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

pollfd*	SocketManager::getPollfd(t_sockfd socket)
{
	if (_socket_to_poll.find(socket) == _socket_to_poll.end())
	{
		cerr << "poll's information at socket [" << socket << "] not found" << endl;
		return (NULL);
	}
	return &_socket_to_poll[socket];
}