/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:32:26 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 19:51:15 by sokaraku         ###   ########.fr       */
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
 * @param manager Reference to the SocketManager instance.
 * @throws std::runtime_error If an error occurs while accepting the connection.
 */
void	SocketPollManager::clientHandler(SocketPollInfo poll_info, SocketManager& manager, short& fd_events)
{
	eventHandler	client_events[] = {
		{ POLLIN, clientPollIn },
		{ POLLOUT, clientPollOut },
		{ POLLHUP, clientPollHup },
		{ POLLERR, clientPollErr }
	};

	for (size_t i = 0; i < (sizeof(client_events) / sizeof(client_events[0])); i++)
	{
		if (poll_info.pfd.revents & client_events[i].event)
		{
			try 
			{
				client_events[i].handler(poll_info, manager, *this);
				if (client_events[i].event == POLLIN && _socket_to_request.at(poll_info.pfd.fd).getIsCompleteRequest() == true)
					fd_events |= POLLOUT;
				if (client_events[i].event == POLLOUT && _socket_to_response[poll_info.pfd.fd].fully_sent == true)
					fd_events &= ~POLLOUT;
			}
			catch (ResponseException& e)
			{
				cerr << "clientHandler(): " << e.what() << endl;
				Response r = e.getResponse();
				clientSend(poll_info, r);
				//manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
				break ;
			}
			catch (exception& e)
			{
				cerr << "(replace me) debug: clientHandler(): " << e.what() << endl;
				//! add 5XX response instead of closing the connection, for server persistence
				manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
				break ;
			}
		}
	}
}


// 1. add exception class with response (sendResponse to the client). All exceptions must be caught in clientHandler and sent to the client (see catch)
// 1. and check if connection must be closed or not
// 1. add getStatus() in send for appropriate error handling

// 2. Implement 100 continue 


/**
 * @brief Receives data from the client.
 * 
 * This function reads data from the client socket and processes the received request.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @param server Reference to the ServerConfig instance.
 */
void	SocketPollManager::clientRecv(SocketPollInfo poll_info, ServerConfig& server)
{
	string		raw_request;
	size_t		client_max_body_size = server.client_max_body_size;
	ssize_t		ret;
	Request		request;
	size_t		total_bytes_read = 0;
	timeval		start, end;

	prepareRecv(poll_info.pfd.fd, total_bytes_read, request, start);
	while (total_bytes_read <= client_max_body_size && ret)
	{
		checkRequestTimeout(start, end);
		int status = 0;
		ret = readOne(poll_info.pfd.fd, raw_request, client_max_body_size, total_bytes_read, status);
		if (ret < 0)
			return recvError(poll_info.pfd.fd, status, request);
		try
		{
			request.parseRequest(raw_request);
		}
		catch (ContinueException)
		{
			//store ResponseException class into the map
			//call clientSend here to inform that the request can continue
			//continue loop
		}
	}
	if (ret == 0)
		request.setIsCompleteRequest(true);
	_socket_to_request[poll_info.pfd.fd] = request;

}


/**
 * @brief Sends the response to the client through the socket.
 * 
 * This function handles the process of sending the generated HTTP response to the client.
 * It retrieves the response from the request handler, converts it to a C-style string,
 * and then sends it through the socket in a non-blocking manner. It also handles potential
 * errors during the send operation and closes the connection after sending the data.
 * 
 * @param poll_info The SocketPollInfo struct containing information about the socket.
 * @param manager The SocketManager instance used to manage the socket connections.
 * @param server The ServerConfig instance containing the server configuration.
 * 
 * @return The number of bytes sent to the client.
 * 
 * @throws runtime_error if the `send` function fails.
 */
ssize_t	SocketPollManager::clientSend(SocketPollInfo& poll_info, SocketManager& manager, ServerConfig& server) //prepare non blocking here too and add try catch
{
	Response	class_response;
	size_t		len_sent = 0;
	timeval		start, end;
	prepareSend(poll_info.pfd.fd, len_sent, class_response, start, server);

	string		string_response = class_response.getResponse();
	char*		buffer = (char *)string_response.c_str();
	size_t		len_response = string_response.size();

	while (len_sent != len_response)
	{
		checkResponseTimeout(start, end);
		ssize_t ret = send(poll_info.pfd.fd, buffer + len_sent, len_response - len_sent, MSG_DONTWAIT);
		int status = checkSocketStatus(poll_info.pfd.fd);

		if (ret < 0)
			return sendError(poll_info.pfd.fd, status, class_response, len_sent), len_sent;
		len_sent += ret;

		if (ret == 0)
			break ;
	}

	string	keepConnection = _socket_to_request.at(poll_info.pfd.fd).getConnectionKeepAlive();
	if (keepConnection == "close")
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
	_socket_to_response.at(poll_info.pfd.fd).fully_sent = true;
	return len_sent;
}

/**
 * @brief Sends the HTTP response to the client socket in a non-blocking manner.
 *
 * This function takes a `SocketPollInfo` object containing information about the client socket
 * and a `Response` object containing the HTTP response to be sent. It converts the response
 * into a C-style string, then uses a loop to send the data in chunks using the `send` function
 * with the `MSG_DONTWAIT` flag to prevent blocking.
 *
 * @param poll_info A `SocketPollInfo` object containing information about the client socket,
 *                  including the file descriptor (`pfd.fd`).
 * @param response A `Response` object containing the HTTP response to be sent to the client.
 * @return The total number of bytes sent to the client.
 * @throws runtime_error if the `send` function returns an error (less than 0).
 *
 * @note The connection is closed after each send operation.  Future implementations should
 *       implement connection keep-alive functionality.
 */
ssize_t	SocketPollManager::clientSend(SocketPollInfo &poll_info, Response& response) //! balls breaking if the send is blocking here
{
	string		string_response = response.getResponse();
	char		*buffer = (char *)string_response.c_str();
	size_t		len_response = string_response.size();
	size_t		len_sent = 0;
	
	while (len_sent != len_response)
	{
		ssize_t ret = send(poll_info.pfd.fd, buffer + len_sent, len_response - len_sent, MSG_DONTWAIT);
		if (ret < 0)
			throw runtime_error("sendData()");
		len_sent += ret;
		if (ret == 0)
			break ;
	}
	//manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET); //*for now, connection close each time. Need to use connection-keep-alive
	return len_sent;
}
