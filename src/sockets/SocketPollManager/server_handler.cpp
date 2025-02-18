/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:31:21 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/18 13:27:20 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

/**
 * @brief Establishes a connection with a client.
 * 
 * This function accepts an incoming connection on the specified server socket
 * and stores the client information. The new client socket is added to the poll
 * structure to monitor events.
 * 
 * @param server_socket The file descriptor of the server socket.
 * @param port The port number on which the server is listening.
 * @throws std::runtime_error If an error occurs while accepting the connection.
 */
void	SocketPollManager::establishConnection(t_sockfd server_socket, int port, SocketManager& manager)
{
	ClientInfo	new_client;

	new_client.client = accept(server_socket, (sockaddr *) &new_client.address, &new_client.size);
	if (new_client.client == -1)
		throw runtime_error(string("SocketPollManager: establishConnection() ") + strerror(errno));
	manager.storeSocket(port, new_client.client, (POLLIN | POLLOUT | POLLERR | POLLHUP), CLIENT_SOCKET, &new_client);
}

void	SocketPollManager::serverHandler(SocketPollInfo poll_info, SocketManager& manager)
{
	cout << POLLIN << " " << poll_info.pfd.revents << endl;
	if (poll_info.pfd.revents & POLLIN)
	{
		try
		{
			establishConnection(poll_info.pfd.fd, poll_info.port, manager);
		}
		catch(exception& e) { cout << e.what() << endl; }
	}
	
	if (poll_info.pfd.revents & POLLERR) //! testing purpose (writing)
	{
		cout << "HO" << endl;
		cerr
		<< "SocketPollManager: serverHandler() error on listening socket [" << poll_info.pfd.fd << "] on port " << poll_info.port
		<< ": " << string(strerror(errno)) << endl;
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, SERVER_SOCKET);
	}

	if (poll_info.pfd.revents & POLLHUP)
	{
		cout << "HA";
		cout << "closing connection on listening socket [" << poll_info.pfd.fd << "] on port " << poll_info.port;
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, SERVER_SOCKET);
	}
}