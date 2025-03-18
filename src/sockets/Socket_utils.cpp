/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:36 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 17:59:38 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

//? check for existing socket in the map before ? Should be ok normally

//TODO store mesage received in raw_request for the constructor of the class Request

/**
 * @brief Closes a connection and removes the socket from the SocketManager.
 * 
 * This function closes the specified socket and removes it from the SocketManager.
 * Depending on the socket type, it either removes a server socket or a client socket.
 * 
 * @param port The port number associated with the socket.
 * @param socket The file descriptor of the socket to be closed.
 * @param type The type of the socket (SERVER_SOCKET or CLIENT_SOCKET).
 */
void	SocketManager::closeConnection(int port, t_sockfd socket_fd, e_SocketType type)
{
	close(socket_fd);
	(type == SERVER_SOCKET) ? removeServerSocket(port) : removeClientSocket(port, socket_fd);
}


static bool operator==(const ClientInfo& c, t_sockfd socket) { return c.client_fd == socket; }
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
void	SocketManager::removeClientSocket(int port, t_sockfd socket_fd)
{
	vector<ClientInfo>&	a_clients = _ports_info[port].clients;

	vector<ClientInfo>::iterator to_del_client = find(a_clients.begin(), a_clients.end(), socket_fd);
	if (to_del_client != a_clients.end())
		a_clients.erase(to_del_client);
	vector<pollfd>::iterator to_del_poll = find(_poll_fds.begin(), _poll_fds.end(), socket_fd);
	if (to_del_poll != _poll_fds.end())
		_poll_fds.erase(to_del_poll);
	_socket_to_poll.erase(socket_fd);
	_poll_manager.removeSocket(socket_fd);
}

/**
 * @brief Removes a server socket from the SocketManager.
 * 
 * This function removes the specified server socket and all associated client sockets
 * from the SocketManager. It also erases the socket from the socket-to-poll mapping.
 * 
 * @param port The port number associated with the server socket.
 */
void	SocketManager::removeServerSocket(int port)
{
	map<int, PortInfo>::iterator	it_port_info = _ports_info.find(port);

	if (it_port_info == _ports_info.end())
		return ;

	for (size_t i = 0; i  < it_port_info->second.clients.size(); i++)
		close(it_port_info->second.clients[i].client_fd);

	vector<pollfd>::iterator	to_del = find(_poll_fds.begin(), _poll_fds.end(), it_port_info->second.server_fd);
	if (to_del != _poll_fds.end())
		_poll_fds.erase(to_del);

	_ports_info.erase(port);
	_socket_to_poll.erase(port);
}

/**
 * @brief Stores a socket in the SocketManager.
 * 
 * This function stores the specified socket in the SocketManager, adding it to the
 * poll structure and the socket-to-poll mapping. If the socket is a client socket,
 * it is also added to the list of client sockets for the specified port.
 * 
 * @param port The port number associated with the socket.
 * @param socket The file descriptor of the socket to be stored.
 * @param options The poll options for the socket.
 * @param type The type of the socket (SERVER_SOCKET or CLIENT_SOCKET).
 * @param client Pointer to the ClientInfo structure, if the socket is a client socket.
 */
void	SocketManager::storeSocket(int port, t_sockfd socket_fd, short options, e_SocketType type, ClientInfo *client)
{
	SocketPollInfo	tmp;

	tmp.pfd.fd = socket_fd;
	tmp.pfd.events = options;
	tmp.pfd.revents = 0;
	tmp.type = type;
	tmp.port = port;

	_poll_fds.push_back(tmp.pfd);
	_socket_to_poll[socket_fd] = tmp;

	if (client != NULL)
		_ports_info[port].clients.push_back(*client);
}


/**
 * @brief Gets the error status of a socket, if any.
 * 
 * This function retrieves the pending error for the socket, which represents
 * the last error encountered by the socket layer.
 * 
 * @param socket_fd The file descriptor of the socket to check.
 * @return int The error status of the socket. Returns 0 if no error is pending.
 */
int		checkSocketStatus(t_sockfd socket_fd)
{
	int			error = 0;
	socklen_t	error_len = sizeof(error);

	getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &error, &error_len);

	return error;
}