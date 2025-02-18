/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:36 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/18 13:29:09 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

//? check for existing socket in the map before ? Should be ok normally

//TODO store mesage received in raw_request for the constructor of the class Request

void	SocketManager::closeConnection(int port, t_sockfd socket, e_SocketType type)
{
	close(socket);
	(type == SERVER_SOCKET) ? removeServerSocket(port) : removeClientSocket(port, socket);
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
void	SocketManager::removeClientSocket(int port, t_sockfd socket)
{
	vector<ClientInfo>&	a_clients = _ports_info[port].clients;

	vector<ClientInfo>::iterator to_del_client = find(a_clients.begin(), a_clients.end(), socket);
	if (to_del_client != a_clients.end()) //!testing purposes
	{
		cout << GREY << "socket [" << socket << "] removed from clients vector" << NC << endl;
		a_clients.erase(to_del_client);
	}

	vector<pollfd>::iterator to_del_poll = find(_poll_fds.begin(), _poll_fds.end(), socket);
	if (to_del_poll != _poll_fds.end()) //!testing purposes
	{
		cout << GREY << "socket [" << socket << "] removed from pollfd vector" << NC << endl;
		_poll_fds.erase(to_del_poll);
	}
	_socket_to_poll.erase(socket);
}

void	SocketManager::removeServerSocket(int port)
{
	map<int, PortInfo>::iterator	it_port_info = _ports_info.find(port);

	if (it_port_info == _ports_info.end())
		return ; //? Is it necessary?

	for (size_t i = 0; i  < it_port_info->second.clients.size(); i++)
		close(it_port_info->second.clients[i].client);

	vector<pollfd>::iterator	to_del = find(_poll_fds.begin(), _poll_fds.end(), it_port_info->second.server);
	if (to_del != _poll_fds.end())
	{
		cout << GREY << "socket [" << it_port_info->second.server << "] removed from pollfd vector" << NC << endl;
		_poll_fds.erase(to_del);
	}

	_ports_info.erase(port);
	_socket_to_poll.erase(port);
}

void	SocketManager::storeSocket(int port, t_sockfd socket, short options, e_SocketType type, ClientInfo *client)
{
	SocketPollInfo	tmp;

	tmp.pfd.fd = socket;
	tmp.pfd.events = options;
	tmp.pfd.revents = 0;
	tmp.type = type;
	tmp.port = port;

	cout << port << " " << socket << " " << options << " ";
	_poll_fds.push_back(tmp.pfd); //*pollfd is copied, so no dangling pointer issue
	_socket_to_poll[socket] = tmp;

	if (client != NULL)
		_ports_info[port].clients.push_back(*client);
}