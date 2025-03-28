/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:56:24 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/28 16:57:07 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

/**
 * @brief Prepares the response to be sent for a given socket file descriptor.
 *
 * This function determines the appropriate server configuration based on the 
 * host and port information extracted from the request header. It either 
 * initializes a new response or retrieves an existing one from the internal 
 * mapping. Additionally, it updates the start time for the response handling.
 *
 * @param socket_fd The socket file descriptor associated with the client request.
 * @param len_sent Reference to the length of data already sent for the response.
 * @param response Reference to the Response object to be prepared or updated.
 * @param start Reference to a timeval structure to store the start time of the response handling.
 * @param servers A vector of ServerConfig objects representing the available server configurations.
 */
void	SocketPollManager::prepareSend(t_sockfd socket_fd, size_t& len_sent, Response& response, timeval& start, vector <ServerConfig>& servers)
{
	map<t_sockfd, infoResponse>::iterator it = _socket_to_response.find(socket_fd);
	ssize_t index = findBestServer(servers, _socket_to_request[socket_fd], true);

	if (it == _socket_to_response.end())
	{
		gettimeofday(&start, NULL);
		response = _socket_to_request[socket_fd].handleRequest(servers[index]);
		return ;
	}

	response = it->second.response;
	len_sent = it->second.len_sent;
	
	gettimeofday(&start, NULL);
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
 * @brief Handles sending an error response to a client socket.
 *
 * This function manages the process of sending an error response to a client
 * through the specified socket file descriptor. If the operation is blocking,
 * it stores the response information for later processing. Otherwise, it throws
 * an exception with a 500 Internal Server Error response.
 *
 * @param socket_fd The file descriptor of the client socket.
 * @param status The status code indicating the result of the operation.
 *               If it equals BLOCKING_OPERATION, the response is stored for later.
 * @param response The Response object containing the error response to be sent.
 * @param len_sent A reference to the size of the response already sent.
 *
 * @throws ResponseException If the status is not BLOCKING_OPERATION, an exception
 *         is thrown with a 500 Internal Server Error response and the corresponding
 *         error message.
 */
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