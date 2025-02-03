/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_operations.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:36 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/03 15:58:20 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

/**
 * @brief Sets the specified socket to non-blocking mode.
 * 
 * The specified socket is set to non-blocking mode using the `fcntl` system call.
 * In non-blocking mode, socket operations such as `read` and `write` will return immediately
 * if they cannot be completed, rather than blocking the calling thread.
 * 
 * @param socket_fd The file descriptor of the socket to set to non-blocking mode.
 * @throws std::runtime_error If an error occurs while setting the socket to non-blocking mode.
 */
void	SocketManager::setToNonBlockingMode(int socket_fd)
{
	int	flags = fcntl(socket_fd, F_GETFL, 0);

	if (flags == -1)
		throw runtime_error(string("SocketManager: ") + strerror(errno));

	if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) //*not using fcntl directly cause it'll overwrite all flags
		throw runtime_error(string("SocketManager: ") + strerror(errno));
	//! testing purposes
	cout << "Socket: " << socket_fd << " set to non blocking mode\n";
}

/**
 * @brief Sets the specified socket to be reusable.
 * 
 * Sets the specified socket to be reusable using the `setsockopt` system call
 * with the `SO_REUSEADDR` option. Enabling this option allows the socket to bind to an address
 * that is already in use, which can be useful for quickly restarting a server without waiting
 * for the socket to be released from the `TIME_WAIT` state.
 * 
 * @param socket The file descriptor of the socket to set to be reusable.
 */
void	SocketManager::setReusability(int socket)
{
	int	opt = 1;
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	//*when a TCP connection closes, the port is kept in TIME_WAIT by the OS (default is 60 seconds)
	//*it prevents delayed packets to interfere with new connections.
	//*By enabling SO_REUSADDR, the port can be reused immediately.
	//*TL;DR if the program is launched, then stopped, and launched immediately after, without SO_REUSEADDR, binding will fail.
}