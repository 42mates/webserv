/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:52:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 19:09:57 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

/**
 * @brief Constructs a SocketPollManager object.
 * 
 * Initializes the SocketPollManager with the provided SocketManager instance.
 * Copies the port information, poll file descriptors, and socket-to-poll mapping
 * from the SocketManager.
 * 
 * @param manager Reference to the SocketManager instance.
 */
SocketPollManager::SocketPollManager(SocketManager& manager)
{
	_ports_info = manager.getPortsInfo();
	_poll_fds = manager.getPollFds();
	_socket_to_poll = manager.getSocketToPoll();
}


SocketPollManager::~SocketPollManager() {}

/**
 * @brief Retrieves the socket information for a given poll file descriptor.
 *
 * This function takes a pollfd structure and returns a SocketPollInfo structure
 * containing the poll file descriptor, port, and type of the socket.
 *
 * @param pfd The poll file descriptor for which the socket information is to be retrieved.
 * @return A SocketPollInfo structure containing the socket information.
 */
SocketPollInfo	SocketPollManager::getSocketInfo(pollfd pfd)
{
	SocketPollInfo	info;

	info.pfd = pfd;
	info.port = _socket_to_poll->at(pfd.fd).port;
	info.type = _socket_to_poll->at(pfd.fd).type;

	return info;
}

/**
 * @brief Gets the map of socket file descriptors to Request objects.
 *
 * This function returns a pointer to the internal map that stores the association
 * between socket file descriptors and Request objects.
 *
 * @return A pointer to the map of socket file descriptors to Request objects.
 */
map<t_sockfd, Request>*	SocketPollManager::getSocketToRequest() 
{
	return &_socket_to_request;
}

/**
 * @brief Gets the map of socket file descriptors to infoResponse objects.
 *
 * This function returns a pointer to the internal map that stores the association
 * between socket file descriptors and infoResponse objects.
 *
 * @return A pointer to the map of socket file descriptors to infoResponse objects.
 */
map<t_sockfd, infoResponse>*	SocketPollManager::getSocketToResponse() 
{
	return &_socket_to_response;
}

/**
 * @brief Removes a socket from the internal maps.
 *
 * This function removes the entries associated with the given socket file descriptor
 * from the internal maps `_socket_to_response` and `_socket_to_request`.
 *
 * @param socket_fd The file descriptor of the socket to be removed.
 */
void	SocketPollManager::removeSocket(t_sockfd socket_fd)
{
	_socket_to_response.erase(socket_fd);
	_socket_to_request.erase(socket_fd);
}


/**
 * @brief Prepares data for a socket before reading a request.
 *
 * This function retrieves the total bytes read and the Request object associated with
 * the given socket file descriptor. If the socket is not found in the internal map,
 * it initializes the start time.
 *
 * @param socket_fd The file descriptor of the socket.
 * @param total_bytes_read A reference to the variable to store the total bytes read.
 * @param request A reference to the variable to store the Request object.
 * @param start A reference to the timeval structure to store the start time.
 */
void	SocketPollManager::prepareRecv(t_sockfd socket_fd, size_t& total_bytes_read, Request& request, timeval& start)
{
	map<t_sockfd, Request>::iterator it = _socket_to_request.find(socket_fd);

	if (it == _socket_to_request.end())
	{
		gettimeofday(&start, NULL);
		return ;
	}

	total_bytes_read = it->second.getRawRequest().size();
	request = it->second;
	gettimeofday(&start, NULL);
}

/**
 * @brief Prepares data for a socket before sending a response.
 *
 * This function retrieves the length sent and the Response object associated with the
 * given socket file descriptor. If the socket is not found in the internal map,
 * it handles the request and generates a response.
 *
 * @param socket_fd The file descriptor of the socket.
 * @param len_sent A reference to the variable to store the length sent.
 * @param response A reference to the variable to store the Response object.
 * @param server A reference to the ServerConfig object.
 */
void	SocketPollManager::prepareSend(t_sockfd socket_fd, size_t& len_sent, Response& response, timeval& start, ServerConfig& server)
{
	map<t_sockfd, infoResponse>::iterator it = _socket_to_response.find(socket_fd);

	if (it == _socket_to_response.end())
	{
		gettimeofday(&start, NULL);
		response = _socket_to_request[socket_fd].handleRequest(server);
		return ;
	}

	response = it->second.response;
	len_sent = it->second.len_sent;
	gettimeofday(&start, NULL);
}