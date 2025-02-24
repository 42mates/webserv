/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:32:26 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/24 14:53:12 by sokaraku         ###   ########.fr       */
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
void	SocketPollManager::clientHandler(SocketPollInfo poll_info, SocketManager& manager)
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
			client_events[i].handler(poll_info, manager, *this);
	}
}

/*
timer starting in loop with a given time to not exceed
*/

/**
 * @brief Reads all available data from the socket.
 * 
 * This function reads data from the specified socket in a non-blocking manner
 * and appends it to the provided raw_request string.
 * 
 * @param socket The file descriptor of the socket to read from.
 * @param raw_request Reference to the string to store the read data.
 * @return The total number of bytes read.
 * @throws std::runtime_error If an error occurs while reading from the socket.
 */
static ssize_t readAll(t_sockfd socket, string& raw_request)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE] = {0};
	ssize_t		bytes_received;

	while (true)
	{
		bytes_received = recv(socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
		cout << "bytes received is " << bytes_received << endl;
		//? Why recv returns -1 when there are already bytes that were read ?
		//! Actual configuration of ifs is wrong, but this is just for testing
		if (bytes_received < 0 && raw_request.empty() == true)
			throw runtime_error("readAll()"); //! COME BACK error. Need to know what happened without using errno (see flags)
		else if (bytes_received < 0)
			break ;
		if (bytes_received == 0)
			break ;
		raw_request.append(buffer, bytes_received);
	}

	return raw_request.size();
}


static ssize_t	readOne(t_sockfd socket_fd, string& request, size_t client_max_body_size, ssize_t& total_bytes_read)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE] = {0};
	ssize_t		bytes_to_read = (client_max_body_size > BUFFER_SIZE ? BUFFER_SIZE : client_max_body_size);
	ssize_t		bytes_received = 0;

	if (total_bytes_read + bytes_to_read > client_max_body_size)
		bytes_to_read = client_max_body_size - total_bytes_read;

	bytes_received = recv(socket_fd, buffer, bytes_to_read, MSG_DONTWAIT);
	request.clear();
	request = buffer;

	if (bytes_received != -1)
		total_bytes_read += bytes_received;
	return bytes_received;
}

// /**
//  * @brief Receives data from the client.
//  * 
//  * This function reads data from the client socket and processes the received request.
//  * 
//  * @param poll_info The poll information containing the file descriptor and events.
//  * @param server Reference to the ServerConfig instance.
//  */
void	SocketPollManager::clientRecv(SocketPollInfo poll_info, ServerConfig& server)
{
	//timer here
	string	raw_request;
	ssize_t	ret = readAll(poll_info.pfd.fd, raw_request);
(void)server;
	//! TESTING PURPOSES
	if (ret > 0)
	{
		cout << "Received data \n" << raw_request << endl;
		_request.parseRequest(raw_request); //except for 100 continue, return error. for 100 : keep request
		_response = _request.handleRequest(server);
	}
	else
		cout << "No data received" << endl;

	// try
	// {
	// 	throw ResponseException(Response("404"));
	// 	throw OneOtherExcteiopn;
	// }
	// catch(OneOtherException& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	// catch(Response100& e)
	// 	keep_class; //keep doing clientRecv if code is 100, send to response class.
	
}
//todo add getsockopt to check for errors like ewouldblock
void	SocketPollManager::clientRecv(SocketPollInfo poll_info, ServerConfig& server)
{
	ssize_t	total_bytes_read = 0;
	string	request;
	ssize_t	client_max_body_size = server.client_max_body_size;
	ssize_t	ret;

	while (total_bytes_read <= client_max_body_size)
	{
		ret = readOne(poll_info.pfd.fd, request, client_max_body_size, total_bytes_read);
		if (ret < 0 && total_bytes_read == 0)
			throw runtime_error("clientRecv() " + string(strerror(errno))); //! REMOVE AND REPLACE BY GETSOCKOPT()
		if (ret == 0)
			break ;
		else
		{
			try 
			{
				_request.parseRequest(request);
			}
		}
	}
}


//todo code for 100
//todo do one clientrecv for one parse request
//todo client_max_body_size OK
//todo set the value of the boolean (all is read or not)
/**
 * @brief Sends data to the client.
 * 
 * This function sends the response data to the client socket in a non-blocking manner.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @return The total number of bytes sent.
 * @throws std::runtime_error If an error occurs while sending data to the socket.
 */
ssize_t	SocketPollManager::clientSend(SocketPollInfo poll_info, SocketManager& manager)
{
	string	response = _response.getResponse();
	char	*buffer = (char *)response.c_str();
	ssize_t	len_response = response.size();
	ssize_t	len_sent = 0;

	cout << "clientSend()" << endl; //!testing purposes
	while (true)
	{
		ssize_t ret = send(poll_info.pfd.fd, buffer + len_sent, len_response - len_sent, MSG_DONTWAIT);
		if (ret < 0)
			throw runtime_error("sendData()");
		if (ret == 0)
			break ;
		len_sent += ret;
	}
	if (_request.getConnectionKeepAlive() == "close")
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
	return len_sent;
}



