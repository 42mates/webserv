/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:31:21 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 13:59:06 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

/**
 * @brief Handles server events based on the poll information.
 * 
 * This function iterates through an array of server events and checks if any of the
 * events have occurred on the provided poll information. If an event has occurred,
 * the corresponding handler function is called.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param manager Reference to the SocketManager instance.
 */
void	SocketPollManager::serverHandler(SocketPollInfo poll_info, SocketManager& manager)
{
	eventHandler	server_events[] = {
		{ POLLIN, serverPollIn },
		{ POLLHUP, serverPollHup },
		{ POLLERR, serverPollErr }
	};

	for (size_t i = 0; i < (sizeof(server_events) / sizeof(server_events[0])); i++)
	{
		if (poll_info.pfd.revents & server_events[i].event)
		{
			try
			{
				server_events[i].handler(poll_info, manager, *this);
			}
			catch (const exception& e)
			{
				cerr << "debug: serverHandler(): " << e.what() << endl;
				manager.closeConnection(poll_info.port, poll_info.pfd.fd, SERVER_SOCKET);
				break ;
			}
		}
	}
}


/**
 * @brief Establishes a connection with a client.
 * 
 * This function accepts an incoming connection on the specified server socket
 * and stores the client information. The new client socket is added to the poll
 * structure to monitor events.
 * 
 * @param server_socket The file descriptor of the server socket.
 * @param port The port number on which the server is listening.
 * @param manager Reference to the SocketManager instance.
 * @throws std::runtime_error If an error occurs while accepting the connection.
 */
void	SocketPollManager::establishConnection(t_sockfd server_socket, int port, SocketManager& manager)
{
	ClientInfo	new_client;

	new_client.client_fd = accept(server_socket, (sockaddr *) &new_client.address, &new_client.size);
	if (new_client.client_fd == -1)
		throw runtime_error(string("SocketPollManager::establishConnection() ") + strerror(errno));
	manager.storeSocket(port, new_client.client_fd, (POLLIN | POLLERR | POLLHUP), CLIENT_SOCKET, &new_client);
}
