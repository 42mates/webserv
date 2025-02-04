/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_init.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:19 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/04 17:29:25 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/**
 * @brief Creates a socket for the specified port index. At this point, the socket
 * is just a file descriptor, and isn't bound to any IP address or port yet.
 * @param port Port's number.
 */
void	SocketManager::createSocket(const string& ip, int port)
{
	if (_ports_info.find(port) != _ports_info.end())
		throw runtime_error(string("SocketManager: createSocket() [") + itostr(port) + "] : " + PORT_ALREADY_IN_USE);

	int	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Address family (IPv4), type of socket, protocol (TCP). 
	if (socket_fd == -1)
		throw runtime_error(string("SocketManager: createSocket() ") + SOCKET_CREATION_ERROR);

	_ports_info[port] = PortInfo();
	_ports_info[port].server = socket_fd;

	_ports_info[port].server_address.sin_family = AF_INET; //same as before, sets the address family as IPv4
	_ports_info[port].server_address.sin_port = htons(port); // sets the bits to big-endian (consistent with network protocols)
	if (inet_pton(AF_INET, ip.c_str(), &_ports_info[port].server_address.sin_addr) <= 0)
		throw runtime_error("SocketManager: createSocket() invalid IP address in " + ip);
}

/**
 * @brief Binds the socket to the specified port index and the associated IP address.
 * Now, the socket is addressable : clients can locate it with the assigned IP/port pair.
 * 
 * Before binding, the socket is set to SO_REUSADDR to avoid a binding fail when the server is restarted
 * (EADDRINUSE).
 * 
 * @param port Port's number.
 */
void	SocketManager::bindSocket(int port)
{
	sockaddr_in&	a_server_address = _ports_info[port].server_address;
	int&			a_server = _ports_info[port].server;

	if (bind(a_server, (struct sockaddr*)(&a_server_address), sizeof(a_server_address)) < 0)
		throw runtime_error(string("SocketManager: bindSocket() ") + SOCKET_BINDING_ERROR);
}

/**
 * @brief Sets a socket to a passive listening state, so that it can wait for 
 * incoming connection requests, using accept(). At this point, the socket can't send
 * or receive data directly, it can just accept requests.
 * @param port Port's number.
 */
void	SocketManager::listenSocket(int port)
{
	//COME BACK connexion queue
	//second parameter is basically a connexion queue, it sets the the maximum number of connection requests that can be queued
	if (listen(_ports_info[port].server, 1) < 0) // the second parameter will probably change
		throw runtime_error(string("SocketManager: listenSocket() ") + SOCKET_LISTENING_ERROR);
}
