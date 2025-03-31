/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:19:55 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/31 11:00:15 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"


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
void	SocketPollManager::prepareRecv(t_sockfd socket_fd, size_t& b_read, size_t& c_max_size, vector<ServerConfig>& servers, Request*& request, timeval& start)
{
	map<t_sockfd, Request*>::iterator it = _socket_to_request.find(socket_fd);

	if (it == _socket_to_request.end())
	{
		gettimeofday(&start, NULL);
		request = new Request();
		_socket_to_request[socket_fd] = request;
		return ;
	}

	ssize_t	index = findBestServer(servers, *it->second, false);
	if (index != -1)
		c_max_size = servers[index].client_max_body_size;
	b_read = it->second->getRawRequest().size();
	request = it->second; 
	gettimeofday(&start, NULL);
}


/**
 * @brief Reads a chunk of data from a socket and processes it.
 * 
 * This function attempts to read a chunk of data from the specified socket
 * into a buffer, appends it to the raw request string, and updates the
 * request object. It also checks for errors, socket status, and ensures
 * that the request body size does not exceed the maximum allowed size.
 * 
 * @param socket_fd The file descriptor of the socket to read from.
 * @param raw_request A reference to a string where the raw request data will be stored.
 * @param client_max_body_size The maximum allowed size for the client's request body.
 * @param total_bytes_read A reference to a variable tracking the total bytes read so far.
 * @param status A reference to an integer that will store the status of the socket operation.
 * 
 * @return The number of bytes received, or -1 if an error occurred.
 */
ssize_t	SocketPollManager::readOneChunk(t_sockfd socket_fd, string& raw_request, size_t client_max_body_size, size_t& total_bytes_read, int& status)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE + 1] = {0};
	ssize_t		bytes_to_read = (client_max_body_size > BUFFER_SIZE ? BUFFER_SIZE : client_max_body_size);
	ssize_t		bytes_received = 0;

	bytes_received = recv(socket_fd, buffer, bytes_to_read, MSG_DONTWAIT);
	status = checkSocketStatus(socket_fd);
	if (bytes_received == -1)
	{
		status == 0 ? status = BLOCKING_OPERATION : status;
		return -1;
	}
	total_bytes_read += bytes_received;
	if (bytes_received > 0)
		buffer[bytes_received] = '\0';

	raw_request.clear();
	raw_request = buffer;

	return bytes_received;
}


/*

check executeScript()
check for testers (timeout and other errors)
*/

/*
getBodySize() returns the expected size of the request
*/



/// Checks if the request body size exceeds the specified limit and throws a ResponseException if it does.
void	checkIfRequestTooLarge(size_t body_size, size_t limit)
{
	if (body_size > limit)
		throw ResponseException(Response("413"), "Request Entity Too Large");
}



/**
 * @brief Checks if the request has timed out.
 *
 * This function checks if the time elapsed since the start of the request exceeds the maximum
 * allowed request timeout (`MCS_REQUEST_TIMEOUT`). If a timeout is detected, it throws a
 * `ResponseException` with a "408 Request Timeout" status.
 *
 * @param start The timeval structure representing the start time of the request.
 * @param end The timeval structure representing the current time.
 *
 * @throws ResponseException If the request has timed out.
 */
void	SocketPollManager::checkRequestTimeout(timeval& start, timeval& end)
{
	gettimeofday(&end, NULL);
	if (isTimeOutReached(start, end, MCS_REQUEST_TIMEOUT) == true)
		throw ResponseException(Response("408"), "Request timeout");
}

/**
 * @brief Handles negative return values from `recv()`.
 *
 * This function handles negative return values from the `recv` function. If the status indicates
 * a blocking operation, it stores the current `request` in the `m_request` map. Otherwise, it
 * throws a `runtime_error` indicating the error that occurred during the `recv` call.
 *
 * @param socket_fd The file descriptor of the socket.
 * @param status The status of the `recv` operation. `BLOCKING_OPERATION` indicates a blocking
 *               operation, otherwise it's an error code.
 * @param request The current request object.
 * @param m_request Reference to the map storing requests associated with socket file descriptors.
 *
 * @throws std::runtime_error If the status is not `BLOCKING_OPERATION`, indicating an error
 *                            during the `recv` call.
 */
void	SocketPollManager::recvError(t_sockfd socket_fd, int& status, Request& request)
{
	if (status == BLOCKING_OPERATION)
		_socket_to_request[socket_fd] = &request;
	else
		throw ResponseException(Response("500"), strerror(status));
}


/**
 * @brief Determines the maximum body size allowed for a client request.
 *
 * This function finds the best matching server configuration for the given
 * request and retrieves the `client_max_body_size` value from it. If no
 * matching server is found, the function returns without modifying the
 * provided parameters.
 *
 * @param servers A vector of server configurations to search through.
 * @param request The client request used to determine the best server match.
 * @param size_set_to_default A reference to a boolean that indicates whether
 *        the size was set to a default value.
 * @param c_mbs A reference to a size_t variable where the client max body size
 *        will be stored if a matching server is found.
 */
void	findClientMaxBodySize(vector<ServerConfig>& servers, Request& request, bool& size_set_to_default, size_t& c_mbs)
{
	if (size_set_to_default == false)
		return ;

	ssize_t	index = findBestServer(servers, request, false);
	if (index == -1)
		return ;
	size_set_to_default = false;
	c_mbs = servers[index].client_max_body_size;
}