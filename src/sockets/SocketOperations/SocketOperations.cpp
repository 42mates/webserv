/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketOperations.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:39:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/13 19:01:29 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/**
 * @file SocketOperations.cpp
 * @brief Implementation of the SocketOperations class for managing socket operations.
 * 
 * This file contains the implementation of the SocketOperations class, which is responsible for
 * creating, binding, listening, setting options, and closing sockets.
 */

# include "SocketOperations.hpp"

/**
 * @brief Creates a socket and initializes the PortInfo structure.
 * 
 * This function creates a socket with the specified IP address and port, and initializes
 * the PortInfo structure with the socket file descriptor and address information.
 * 
 * @param ip The IP address to bind the socket to.
 * @param port The port number to bind the socket to.
 * @param port_info The PortInfo structure to initialize.
 * @throws std::runtime_error If an error occurs while creating the socket.
 */
void SocketOperations::createSocket(const std::string& ip, int port, PortInfo& port_info)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1)
        throw std::runtime_error(string("SocketOperations: createSocket() ") +  SOCKET_CREATION_ERROR);

    port_info.server = socket_fd;
    port_info.server_address.sin_family = AF_INET;
    port_info.server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &port_info.server_address.sin_addr) <= 0)
        throw std::runtime_error("SocketOperations: createSocket() invalid IP address in " + ip);
}


/**
 * @brief Binds the socket to the specified port and address.
 * 
 * This function binds the socket to the specified port and address stored in the PortInfo structure.
 * 
 * @param port The port number to bind the socket to.
 * @param port_info The PortInfo structure containing the socket and address information.
 * @throws std::runtime_error If an error occurs while binding the socket.
 */
void SocketOperations::bindSocket(PortInfo& port_info)
{
    if (bind(port_info.server, (struct sockaddr*)(&port_info.server_address), sizeof(port_info.server_address)) < 0)
        throw std::runtime_error(string("SocketOperations: bindSocket() ") + SOCKET_BINDING_ERROR);
}


/**
 * @brief Sets the socket to listen for incoming connections.
 * 
 * This function sets the socket to listen for incoming connections on the specified port.
 * 
 * @param port The port number to listen on.
 * @param port_info The PortInfo structure containing the socket information.
 * @throws std::runtime_error If an error occurs while setting the socket to listen.
 */
void SocketOperations::listenSocket(PortInfo& port_info)
{
    if (listen(port_info.server, 1) < 0)//COME BACK connexion queue
        throw std::runtime_error(string("SocketOperations: listenSocket() ") +  SOCKET_LISTENING_ERROR);
}


/**
 * @brief Sets the specified socket to non-blocking mode.
 * 
 * This function sets the specified socket to non-blocking mode using the `fcntl` system call.
 * In non-blocking mode, socket operations such as `read` and `write` will return immediately
 * if they cannot be completed, rather than blocking the calling thread.
 * 
 * @param socket The file descriptor of the socket to set to non-blocking mode.
 * @throws std::runtime_error If an error occurs while setting the socket to non-blocking mode.
 */
void SocketOperations::setToNonBlockingMode(t_sockfd socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags == -1)
		throw std::runtime_error(string("SocketOperations: setToNonBlockingMode() ") + strerror(errno));

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error(string("SocketOperations: setToNonBlockingMode() ") + strerror(errno));

	//! testing purposes
	cout << "Socket: " << socket << " set to non blocking mode\n";
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
}

//! might be useless
/**
 * @brief Closes the specified socket.
 * 
 * This function closes the specified socket using the `close` system call.
 * 
 * @param socket The file descriptor of the socket to close.
 * @throws std::runtime_error If an error occurs while closing the socket.
 */
void SocketOperations::closeSocket(t_sockfd socket)
{
    if (close(socket) == -1)
        throw std::runtime_error("SocketOperations: closeSocket() " + string(strerror(errno)));
}