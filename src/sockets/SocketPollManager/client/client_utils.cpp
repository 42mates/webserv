/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:19:55 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/25 13:37:23 by mbecker          ###   ########.fr       */
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


/**
 * @brief Reads a portion of data from the socket in a non-blocking manner.
 *
 * This function reads a portion of data from the specified socket. The amount of data read
 * is determined by the `client_max_body_size` and `BUFFER_SIZE`. The read data is stored in the
 * `raw_request` string. It also checks for socket errors and sets the `status` accordingly.
 *
 * @param socket_fd The file descriptor of the socket to read from.
 * @param raw_request Reference to the string to store the read data.
 * @param client_max_body_size The maximum allowed size of the client's body.
 * @param total_bytes_read Reference to the total number of bytes read so far.
 * @param status Reference to an integer to store the status of the read operation.
 *               It can be 0 for success, `BLOCKING_OPERATION` if the operation would block,
 *               or an error code from `checkSocketStatus`.
 *
 * @return The number of bytes read in this call, or -1 if an error occurred.
 */
ssize_t	SocketPollManager::readOneChunk(t_sockfd socket_fd, string& raw_request, size_t client_max_body_size, size_t& total_bytes_read, int& status)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE + 1] = {0};
	ssize_t		bytes_to_read = (client_max_body_size > BUFFER_SIZE ? BUFFER_SIZE : client_max_body_size);
	ssize_t		bytes_received = 0;

	// if (static_cast<size_t>(total_bytes_read + bytes_to_read) > client_max_body_size)
	// 	bytes_to_read = client_max_body_size - total_bytes_read;

	if (static_cast<size_t>(total_bytes_read + bytes_to_read) > client_max_body_size)
		throw ResponseException(Response("413"), "Request Entity Too Large");

	bytes_received = recv(socket_fd, buffer, bytes_to_read, MSG_DONTWAIT);

	status = checkSocketStatus(socket_fd);
	if (bytes_received == -1)
	{
		status == 0 ? status = BLOCKING_OPERATION : status;
		return -1;
	}
	if (bytes_received > 0)
		buffer[bytes_received] = '\0';

	raw_request.clear();
	raw_request = buffer;
	
	//! removed condition (doesnt depend on the existence of a limit or not anymore. Check running of loop in clientRecv())
	total_bytes_read += bytes_received;

	return bytes_received;
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
 * @brief Checks if the response has timed out.
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
void	SocketPollManager::checkResponseTimeout(timeval& start, timeval& end)
{
	gettimeofday(&end, NULL);
	if (isTimeOutReached(start, end, MCS_RESPONSE_TIMEOUT) == true)
		throw ResponseException(Response("504"), "Response timeout");
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
		_socket_to_request[socket_fd] = request;
	else
		throw ResponseException(Response("500"), strerror(status));
}


void	SocketPollManager::sendError(t_sockfd socket_fd, int& status, Response& response, size_t&  len_sent)
{
	infoResponse	tmp;

	tmp.response = response;
	tmp.len_sent = len_sent;
	tmp.fully_sent = false;

	if (status == BLOCKING_OPERATION)
		_socket_to_response[socket_fd] = tmp;
	else
		throw ResponseException(Response("500"), strerror(status));
}


bool	SocketPollManager::isMessageEnd(t_sockfd socket_fd, __int8_t type)
{
	if (type == T_REQUEST)
	{
		map<t_sockfd, Request>::iterator it = _socket_to_request.find(socket_fd);
		if (it == _socket_to_request.end())
			return false;
		if (it->second.getIsCompleteRequest() == true)
			return true;
		return false;
	}
	//* type is T_RESPONSE
	map<t_sockfd, infoResponse>::iterator it = _socket_to_response.find(socket_fd);
	if (it == _socket_to_response.end())
		return false;
	if (it->second.fully_sent == true)
		return true;
	return false;

}


bool	keepConnectionOpen(Response& r)
{
	string	status = r.getCode();

	if (status == "400" || status == "413" || status == "500")
		return false;
	return true;
}