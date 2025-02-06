/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_operations.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:44:51 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/06 20:51:22 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

void	SocketManager::runPoll()
{
	int	ret;

	while (true)
	{
		ret = poll(&_poll_fds[0], _poll_fds.size(), 60 * 1000); // one min
		if (ret < 0)
			throw runtime_error(string("SocketManager(): runPoll()") + strerror(errno));
		if (ret == 0)
			continue ;
		for (size_t i = 0; i < _poll_fds.size() && ret; i++)
		{
			if (_poll_fds[i].revents == 0)
				continue ;
			handleClient(_poll_fds[i].fd, _poll_fds[i].revents);
			ret--;
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
 * @throws std::runtime_error If an error occurs while accepting the connection.
 */
void	SocketManager::establishConnection(t_sockfd server_socket, int port)
{
	ClientInfo	new_client;

	new_client.client = accept(server_socket, (sockaddr *) &new_client.address, &new_client.size);
	if (new_client.client == -1)
		throw runtime_error(string("SocketManager: establishConnection() ") + strerror(errno));
	storeSocket(port, new_client.client, (POLLIN | POLLOUT | POLLERR | POLLHUP), CLIENT_SOCKET, &new_client);
}

void	SocketManager::handleClient(t_sockfd socket, int action)
{
	(void)socket, action;
	return ;
}

void	SocketManager::closeConnection(int port, t_sockfd socket)
{
	close(socket);
	removeSocket(port, socket);
}

static bool operator==(const ClientInfo& c, t_sockfd socket) { return c.client == socket; }
static bool operator==(const pollfd& s, t_sockfd socket) { return s.fd == socket; }


/**
 * @brief Removes a socket from the SocketManager.
 * 
 * This function removes the specified socket from the list of client sockets
 * and the poll structure. It also erases the socket from the socket-to-poll mapping.
 * 
 * @param port The port number associated with the socket.
 * @param socket The file descriptor of the socket to be removed.
 */
void	SocketManager::removeSocket(int port, t_sockfd socket)
{
	vector<ClientInfo>&	a_clients = _ports_info[port].clients;

	vector<ClientInfo>::iterator to_del_client = find(a_clients.begin(), a_clients.end(), socket);
	if (to_del_client != a_clients.end())
	{
		cout << GREY << "socket [" << socket << "] removed from clients vector" << NC << endl;
		a_clients.erase(to_del_client);
	}

	vector<pollfd>::iterator to_del_poll = find(_poll_fds.begin(), _poll_fds.end(), socket);
	if (to_del_poll != _poll_fds.end())
	{
		cout << GREY << "socket [" << socket << "] removed from pollfd vector" << NC << endl;
		_poll_fds.erase(to_del_poll);
	}

	_socket_to_poll.erase(socket);
}
