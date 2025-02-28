/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:32:26 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/28 13:16:07 by mbecker          ###   ########.fr       */
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
				if (client_events[i].event == POLLIN)
					fd_events |= POLLOUT;
				if (client_events[i].event == POLLOUT)
					fd_events &= ~POLLOUT;
			}
			catch (exception& e)
			{
				cerr << "SocketPollManager::clientHandler() : " << e.what() << endl;
				manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
				break ;
			}
		}
	}
}


/**
 * @brief Reads a portion of data from the socket.
 * 
 * This function reads a portion of data from the specified socket in a non-blocking manner.
 * The amount of data read is determined by the client_max_body_size and BUFFER_SIZE.
 * The read data is stored in the request string.
 * 
 * @param socket_fd The file descriptor of the socket to read from.
 * @param request Reference to the string to store the read data.
 * @param client_max_body_size The maximum allowed size of the client's body.
 * @param total_bytes_read Reference to the total number of bytes read so far.
 * @return The number of bytes read in this call.
 */
static ssize_t	readOne(t_sockfd socket_fd, string& raw_request, size_t client_max_body_size, ssize_t& total_bytes_read)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE] = {0};
	ssize_t		bytes_to_read = (client_max_body_size > BUFFER_SIZE ? BUFFER_SIZE : client_max_body_size);
	ssize_t		bytes_received = 0;

	if (static_cast<size_t>(total_bytes_read + bytes_to_read) > client_max_body_size)
		bytes_to_read = client_max_body_size - total_bytes_read;

	bytes_received = recv(socket_fd, buffer, bytes_to_read, MSG_DONTWAIT);
	raw_request.clear();
	raw_request = buffer;

	if (bytes_received != -1 && client_max_body_size != string::npos)
		total_bytes_read += bytes_received;
	return bytes_received;
}

//todo timer
//todo add getsockopt to check for errors like ewouldblock

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
	ssize_t		total_bytes_read = 0;
	string		raw_request;
	ssize_t		client_max_body_size = server.client_max_body_size;
	ssize_t		ret;
	Request		request;
	Response	response;

	while (total_bytes_read <= client_max_body_size)
	{
		ret = readOne(poll_info.pfd.fd, raw_request, client_max_body_size, total_bytes_read);
		if (ret < 0 && total_bytes_read == 0)
			throw runtime_error("clientRecv() " + string(strerror(errno))); //! REMOVE AND REPLACE BY GETSOCKOPT()
		else if (ret <= 0)
			break ;
		total_bytes_read == client_max_body_size ? request.setIsCompleteRequest(true) : (void)0;
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
	if (response.getStatus() == "DEFAULT")
		response = request.handleRequest(server);

	cout << response.getStatus() << " " << response.getReason() << endl; //debug type de reponse

	_socket_to_response[poll_info.pfd.fd] = response;
}

/**
 * @brief Sends data to the client.
 * 
 * This function sends the response data to the client socket in a non-blocking manner.
 * 
 * @param poll_info The poll information containing the file descriptor and events.
 * @return The total number of bytes sent.
 * @throws std::runtime_error If an error occurs while sending data to the socket.
 */
ssize_t	SocketPollManager::clientSend(SocketPollInfo& poll_info, SocketManager& manager)
{
	Response	*class_response = &_socket_to_response[poll_info.pfd.fd]; 
	string		string_response = class_response->getResponse();
	char		*buffer = (char *)string_response.c_str();
	ssize_t		len_response = string_response.size();
	ssize_t		len_sent = 0;

	while (len_sent != len_response)
	{
		ssize_t ret = send(poll_info.pfd.fd, buffer + len_sent, len_response - len_sent, MSG_DONTWAIT);
		if (ret < 0)
			throw runtime_error("sendData()");
		len_sent += ret;
		if (ret == 0)
			break ;
	}
	manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET); //*for now, connection close each time. Need to use connection-keep-alive
	return len_sent;
}