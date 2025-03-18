/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:20:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/26 15:11:19 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

/**
 * @brief Handles the POLLIN event for a server socket.
 * 
 * This function is called when there is an incoming connection on the server socket.
 * It delegates the task of establishing the connection to the `establishConnection`
 * method of the `SocketPollManager` class.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	serverPollIn(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	poll_manager.establishConnection(poll_info.pfd.fd, poll_info.port, manager);	
}

/**
 * @brief Handles the POLLHUP event for a server socket.
 * 
 * This function is called when a hang-up occurs on the server socket.
 * It closes the connection associated with the server socket.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	serverPollHup(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	(void)poll_manager;

	cout
	<< MAGENTA << "closing connection on listening socket [" << NC << poll_info.pfd.fd
	<< MAGENTA << "] on port " << NC << poll_info.port;
	manager.closeConnection(poll_info.port, poll_info.pfd.fd, SERVER_SOCKET);
}

/**
 * @brief Handles the POLLERR event for a server socket.
 * 
 * This function is called when an error occurs on the server socket.
 * It logs the error and closes the connection associated with the server socket.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 * @param poll_manager Reference to the SocketPollManager instance.
 */
void	serverPollErr(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager)
{
	(void)poll_manager;
//! issue with errno here too like in clientPollErr?
	cerr
	<< BRED << "SocketPollManager::serverHandler() error on listening socket [" << NC << poll_info.pfd.fd
	<< BRED << "] on port " << NC << poll_info.port
	<< ": " << string(strerror(errno)) << endl;
	manager.closeConnection(poll_info.port, poll_info.pfd.fd, SERVER_SOCKET);
}
