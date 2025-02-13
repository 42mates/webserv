/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:32:26 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/13 20:43:09 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SocketPollManager.hpp"
# include "SocketManager.hpp"

static ssize_t readAll(t_sockfd socket, string& raw_request)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_received;

	while (true)
	{
		bytes_received = recv(socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
		if (bytes_received == -1)
			throw runtime_error("readAll()"); //! COME BACK error. Need to know what happened without usign errno (see flags)
		if (bytes_received == 0)
			break ;
		raw_request.append(buffer, bytes_received);
	}

	return raw_request.size();
}

static void	receiveData(SocketPollInfo poll_info, Request& request)
{
	string	raw_request;
	ssize_t	ret = readAll(poll_info.pfd.fd, raw_request);

	//! TESTING PURPOSES
	if (ret > 0)
	{
		cout << "Received data " << raw_request << endl;
		request.parseRequest(raw_request); 
	}
	else
		cout << "No data received" << endl;
}

static void	sendData()
{
	return ;
}

void	SocketPollManager::clientHandler(SocketPollInfo poll_info, SocketManager& manager)
{
	if (poll_info.pfd.revents & POLLIN)
		receiveData(poll_info, _request);

	if (poll_info.pfd.revents & POLLOUT)
		sendData();

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

