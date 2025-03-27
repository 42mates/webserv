/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketOperations.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:39:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/27 17:27:02 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/**
 * @file SocketOperations.cpp
 * @brief Implementation of the SocketOperations class for managing socket operations.
 * 
 * This file contains the implementation of the SocketOperations class, which is responsible for
 * creating, binding, listening, setting options, and closing sockets.
 */

#include "SocketOperations.hpp"

/**
 * @brief Creates a socket and initializes the PortInfo structure.
 * 
 * This function creates a socket with the specified IP address and port, and initializes
 * the PortInfo structure with the socket file descriptor and address information.
 * 
 * @param ip The IP address to bind the socket to.
 * @param port The port number to bind the socket to.
 * @param port_info The PortInfo structure to initialize.
 * @throws runtime_error If an error occurs while creating the socket.
 */
void SocketOperations::createSocket(const string& ip, int port, PortInfo& port_info)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1)
        throw runtime_error(string("SocketOperations: createSocket() ") +  SOCKET_CREATION_ERROR);

    port_info.server_fd = socket_fd;
    port_info.server_address.sin_family = AF_INET;
    port_info.server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &port_info.server_address.sin_addr) <= 0)
        throw runtime_error("SocketOperations: createSocket() invalid IP address in " + ip);
}


/**
 * @brief Binds the socket to the specified port and address.
 * 
 * This function binds the socket to the specified port and address stored in the PortInfo structure.
 * 
 * @param port The port number to bind the socket to.
 * @param port_info The PortInfo structure containing the socket and address information.
 * @throws runtime_error If an error occurs while binding the socket.
 */
void SocketOperations::bindSocket(PortInfo& port_info, int port)
{
	int8_t	max_try = 5;
	int		bind_return = 0;

	if (!run_server)
		return ;
	while (max_try)
	{
		if (!run_server)
			throw runtime_error(string("webserv: server stopped"));
		bind_return = bind(port_info.server_fd, (struct sockaddr*)(&port_info.server_address), sizeof(port_info.server_address));
		if (bind_return == 0)
			return ;
		char	ip_address[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &port_info.server_address.sin_addr, ip_address, INET_ADDRSTRLEN);
		cerr
		<< "webserv: [emerg] bind() to " << ip_address << ":" << itostr(port)
		<< " failed (" << itostr(errno) << ": " << strerror(errno) << ')' << endl;
		max_try--;
		sleep(1);
	}
	throw runtime_error(string("SocketOperations::bindSocket() ") + SOCKET_BINDING_ERROR);
}


/**
 * @brief Sets the socket to listen for incoming connections.
 * 
 * This function sets the socket to listen for incoming connections on the specified port.
 * 
 * @param port The port number to listen on.
 * @param port_info The PortInfo structure containing the socket information.
 * @throws runtime_error If an error occurs while setting the socket to listen.
 */
void SocketOperations::listenSocket(PortInfo& port_info)
{
    if (listen(port_info.server_fd, 10) < 0)//COME BACK connexion queue
        throw runtime_error(string("SocketOperations::listenSocket() ") +  SOCKET_LISTENING_ERROR);
}


/**
 * @brief Sets the socket options for a given socket.
 * 
 * This function sets the socket to non-blocking mode and sets the close-on-exec flag.
 * 
 * @param socket The socket file descriptor.
 * @throw runtime_error if fcntl fails.
 */
void SocketOperations::setOptions(t_sockfd socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags == -1)
		throw runtime_error(string("SocketOperations::setToNonBlockingMode() ") + strerror(errno));

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC) == -1)
		throw runtime_error(string("SocketOperations::setToNonBlockingMode() ") + strerror(errno));
}


/**
 * @brief Sets the specified socket to be reusable.
 * 
 * This function sets the specified socket to be reusable using the `setsockopt` system call
 * with the `SO_REUSEADDR` option. Enabling this option allows the socket to bind to an address
 * that is already in use, which can be useful for quickly restarting a server without waiting
 * for the socket to be released from the `TIME_WAIT` state.
 * 
 * @param socket The file descriptor of the socket to set to be reusable.
 */
void SocketOperations::setReusability(t_sockfd socket)
{
    int opt = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
}
