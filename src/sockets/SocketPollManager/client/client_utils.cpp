/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:19:55 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 19:25:14 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

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
ssize_t	SocketPollManager::readOne(t_sockfd socket_fd, string& raw_request, size_t client_max_body_size, size_t& total_bytes_read, int& status)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE + 1] = {0};
	ssize_t		bytes_to_read = (client_max_body_size > BUFFER_SIZE ? BUFFER_SIZE : client_max_body_size);
	ssize_t		bytes_received = 0;

	if (static_cast<size_t>(total_bytes_read + bytes_to_read) > client_max_body_size)
		bytes_to_read = client_max_body_size - total_bytes_read;

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
	
	//! removed condition (doesnt depend on the existence of a limit or not anymore. Check running of loop)
	total_bytes_read += bytes_received;

	return bytes_received;
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
	else //error_handler here for response throwing
		throw runtime_error("clientRecv() " + string(strerror(status)));
}

void	SocketPollManager::sendError(t_sockfd socket_fd, int& status, Response& response, size_t&  len_sent)
{
	infoResponse	tmp;

	tmp.response = response;
	tmp.len_sent = len_sent;

	if (status == BLOCKING_OPERATION)
		_socket_to_response[socket_fd] = tmp;
	else //error_handler here for response throwing
		throw runtime_error("clientSend() " + string(strerror(status)));		
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