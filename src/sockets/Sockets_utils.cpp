/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:36 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/06 19:57:43 by sokaraku         ###   ########.fr       */
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
 * @param socket The file descriptor of the socket to set to non-blocking mode.
 * @throws std::runtime_error If an error occurs while setting the socket to non-blocking mode.
 */
void	SocketManager::setToNonBlockingMode(t_sockfd socket)
{
	int	flags = fcntl(socket, F_GETFL, 0);

	if (flags == -1)
		throw runtime_error(string("SocketManager: setToNonBlockingMode() ") + strerror(errno));

	if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) //*not using fcntl directly cause it'll overwrite all flags
		throw runtime_error(string("SocketManager: setToNonBlockingMode() ") + strerror(errno));
	//! testing purposes
	cout << "Socket: " << socket << " set to non blocking mode\n";
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
void	SocketManager::setReusability(t_sockfd socket)
{
	int	opt = 1;
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	//*when a TCP connection closes, the port is kept in TIME_WAIT by the OS (default is 60 seconds)
	//*it prevents delayed packets to interfere with new connections.
	//*By enabling SO_REUSADDR, the port can be reused immediately.
	//*TL;DR if the program is launched, then stopped, and launched immediately after, without SO_REUSEADDR, binding will fail.
}

//? check for existing socket in the map before ? Should be ok normally

void	SocketManager::storeSocket(int port, t_sockfd socket, short options, e_SocketType type, ClientInfo *client)
{
	SocketPollInfo	tmp;

	tmp.pfd.fd = socket;
	tmp.pfd.events = options;
	tmp.pfd.revents = 0; //! here for testing purposes. Careful when running program
	tmp.type = type;

	_poll_fds.push_back(tmp.pfd); //*pollfd is copied, so no dangling pointer issue
	_socket_to_poll[socket] = tmp;

	if (client != NULL)
		_ports_info[port].clients.push_back(*client);
}