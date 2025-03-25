/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:50:23 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/19 15:38:01 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"


//TODO use getBestServer to find the server associated for a given socket (avoid issues with multiple server block and same port)
/**
 * @brief Handles the POLLIN event for a client socket.
 * 
 * This function is called when there is data to be read from the client socket.
 * It delegates the task of receiving data to the `clientRecv` method of the
 * `SocketPollManager` class.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	clientPollIn(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	ServerConfig*	server = manager.getPortInfo(poll_info.port)->server;
	poll_manager.clientRecv(poll_info, *server);
}

/**
 * @brief Handles the POLLOUT event for a client socket.
 * 
 * This function is called when the client socket is ready for writing.
 * It delegates the task of sending data to the `clientSend` method of the
 * `SocketPollManager` class.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	clientPollOut(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	ServerConfig*	server = manager.getPortInfo(poll_info.port)->server;
	poll_manager.clientSend(poll_info, manager, *server);
}

/**
 * @brief Handles the POLLHUP event for a client socket.
 * 
 * This function is called when a hang-up occurs on the client socket.
 * It closes the connection associated with the client socket.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	clientPollHup(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	(void)poll_manager;
	manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
}

/**
 * @brief Handles the POLLERR event for a client socket.
 * 
 * This function is called when an error occurs on the client socket.
 * It logs the error and closes the connection associated with the client socket.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	clientPollErr(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	(void)poll_manager;
	manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
}