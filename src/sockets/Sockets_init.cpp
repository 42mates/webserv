/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_init.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:19 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/03 15:56:12 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/**
 * @brief Creates a socket for the specified port index. At this point, the socket
 * is just a file descriptor, and isn't bound to any IP address or port yet.
 * @param index Vector's index.
 * @param port Port's value.
 */
void	SocketManager::createSocket(int port)
{
	if (_ports_info.find(port) != _ports_info.end())
		throw runtime_error(string("[") + itostr(port) + "] : " + PORT_ALREADY_IN_USE);

	struct sockaddr_in&	a_server_address = _ports_info[port].server_address;

	int	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Address family (IPv4), type of socket, protocol (TCP). 
	if (socket_fd == -1)
		throw runtime_error(SOCKET_CREATION_ERROR);

	_ports_info[port].server_socket = socket_fd;

	a_server_address.sin_family = AF_INET; //same as before, sets the address family as IPv4
	a_server_address.sin_port = htons(port); // sets the bits to big-endian (consistent with network protocols)
	a_server_address.sin_addr.s_addr = INADDR_ANY; // COME BACK INADDR_ANY = server accessible from all interfaces
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
	int&			a_server_socket = _ports_info[port].server_socket;

	if (bind(a_server_socket, (struct sockaddr*)(&a_server_address), sizeof(a_server_address)) < 0)
		throw runtime_error(SOCKET_BINDING_ERROR);
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
	if (listen(_ports_info[port].server_socket, 1) < 0) // the second parameter will probably change
		throw runtime_error(SOCKET_LISTENING_ERROR);
}
