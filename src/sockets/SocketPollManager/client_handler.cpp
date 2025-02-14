/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:32:26 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/14 14:21:21 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"


/*
timer starting in loop with a given time to not exceed
*/
ssize_t readAll(t_sockfd socket, string& raw_request)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE] = {0};
	ssize_t		bytes_received;

	while (true)
	{
		bytes_received = recv(socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
		if (bytes_received < 0)
			throw runtime_error("readAll()"); //! COME BACK error. Need to know what happened without using errno (see flags)
		if (bytes_received == 0)
			break ;
		raw_request.append(buffer, bytes_received);
	}

	return raw_request.size();
}

void	SocketPollManager::clientRecv(SocketPollInfo poll_info)
{
	string	raw_request;
	ssize_t	ret = readAll(poll_info.pfd.fd, raw_request);

	//! TESTING PURPOSES
	if (ret > 0)
	{
		cout << "Received data " << raw_request << endl;
		_request.parseRequest(raw_request); 
		// _response = _request.handleRequest();
	}
	else
		cout << "No data received" << endl;
}

ssize_t	SocketPollManager::clientSend(SocketPollInfo poll_info)
{
	string	response = _response.getResponse();
	char	*buffer = (char *)response.c_str();
	ssize_t	len_response = response.size();
	ssize_t	len_sent = 0;

	while (true)
	{
		ssize_t ret = send(poll_info.pfd.fd, buffer + len_sent, len_response - len_sent, MSG_DONTWAIT);
		if (ret < 0)
			throw runtime_error("sendData()");
		if (ret == 0)
			break ;
		len_sent += ret;
	}
	return len_sent;
}


void	SocketPollManager::clientHandler(SocketPollInfo poll_info, SocketManager& manager)
{
	string test("test"); //! testing purposes

	if (poll_info.pfd.revents & POLLIN)
	{
		try
		{
			clientRecv(poll_info);
		}
		catch(exception& e) { cout << e.what() << endl; }
	}

	if (poll_info.pfd.revents & POLLOUT)
	{
		try
		{
			clientSend(poll_info);
		}
		catch(exception& e) { cout << e.what() << endl; }
	}

	if (poll_info.pfd.revents & POLLHUP)
	{
		cout << "closing connection on client socket [" << poll_info.pfd.fd << "] on port " << poll_info.port;
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
	}

	if (poll_info.pfd.revents & POLLERR)
	{
		cerr
		<< "SocketPollManager: clientHandler() error on client socket [" << poll_info.pfd.fd << "] on port " << poll_info.port
		<< ": " << string(strerror(errno)) << endl;
		manager.closeConnection(poll_info.port, poll_info.pfd.fd, CLIENT_SOCKET);
	}
}

