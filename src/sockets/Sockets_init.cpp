/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_init.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:19 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/31 11:36:19 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/**
 * @brief Creates a socket for the specified port index. At this point, the socket
 * is just a file descriptor, and isn't bound to any IP address or port yet.
 * @param port_at_index The index of the port to which the socket is to be created.
 */
void	SocketManager::createSocket(int index, int port_at_index)
{
	if (static_cast<size_t>(index) >= _ports_info.size())
		_ports_info.resize(index + 1);

	struct sockaddr_in&	r_server_address = _ports_info[index].server_address;

	int	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Address family (IPv4), type of socket, protocol (TCP). 
	if (socket_fd == -1)
		throw runtime_error(SOCKET_CREATION_ERROR);

	_ports_info[index].server_socket = socket_fd;
	_ports_info[index].port = port_at_index;

	r_server_address.sin_family = AF_INET; //same as before, sets the address family as IPv4
	r_server_address.sin_port = htons(_ports_info[index].port); // sets the bits to big-endian (consistent with network protocols)
	r_server_address.sin_addr.s_addr = INADDR_ANY; // COME BACK INADDR_ANY = server accessible from all interfaces
}

/**
 * @brief Binds the socket to the specified port index and the associated IP address.
 * Now, the socket is addressable : clients can locate it with the assigned IP/port pair.
 * 
 * Before binding, the socket is set to SO_REUSADDR to avoid a binding fail when the server is restarted
 * (EADDRINUSE).
 * 
 * @param index index of the port to which the socket is to be bound.
 */
void	SocketManager::bindSocket(int index)
{
	sockaddr_in&	r_server_address = _ports_info[index].server_address;
	int&			r_server_socket = _ports_info[index].server_socket;

	int	opt = 1;
	setsockopt(r_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//*when a TCP connection closes, the port is kept in TIME_WAIT by the OS (default is 60 seconds)
	//*it prevents delayed packets to interfere with new connections.
	//*By enabling SO_REUSADDR, the port can be reused immediately.
	//*i.e. if the program is launched, then stopped, and launched immediately after, without SO_REUSEADDR, binding will fail.
	if (bind(r_server_socket, (struct sockaddr*)(&r_server_address), sizeof(r_server_address)) < 0)
		throw runtime_error(SOCKET_BINDING_ERROR);
}

/**
 * @brief Sets a socket to a passive listening state, so that it can wait for 
 * incoming connection requests, using accept(). At this point, the socket can't send
 * or receive data directly, it can just accept requests.
 * @param index The index of the port for which the socket is to listen.
 */
void	SocketManager::listenSocket(int index)
{
	//COME BACK connexion queue
	//second parameter is basically a connexion queue, it sets the the maximum number
	// of connection requests that can be queued
	if (listen(_ports_info[index].server_socket, 1) < 0) // the second parameter will probably change
		throw runtime_error(SOCKET_LISTENING_ERROR);
}
