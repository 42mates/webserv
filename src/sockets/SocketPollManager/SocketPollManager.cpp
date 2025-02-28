/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:52:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/28 15:24:44 by sokaraku         ###   ########.fr       */
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

std::ostream& operator<<(std::ostream& os, const SocketPollManager& spm)
{
    os << "Poll FDs:" << std::endl;
    for (size_t i = 0; i < spm._poll_fds->size(); ++i)
    {
        const pollfd& pfd = spm._poll_fds->at(i);
        os << "FD: " << pfd.fd << ", Events: " << pfd.events << ", Revents: " << pfd.revents << std::endl;
    }

    os << "Socket to Poll Mapping:" << std::endl;
    for (std::map<t_sockfd, SocketPollInfo>::const_iterator it = spm._socket_to_poll->begin(); it != spm._socket_to_poll->end(); ++it)
    {
        const SocketPollInfo& spi = it->second;
        os << "Socket FD: " << it->first << ", Poll FD: " << spi.pfd.fd << ", Poll Events: " << spi.pfd.events << ", Poll Revents: " << spi.pfd.revents << ", Type: " << (spi.type == SERVER_SOCKET ? "Server" : "Client") << std::endl;
    }

    return os;
}


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

map<t_sockfd, Request>*	SocketPollManager::getSocketToRequest() 
{
	return &_socket_to_request;
}

map<t_sockfd, Response>*	SocketPollManager::getSocketToResponse() 
{
	return &_socket_to_response;
}

void	SocketPollManager::removeSocket(t_sockfd socket_fd)
{
	_socket_to_response.erase(socket_fd);
}


/**
 * @brief Runs the poll loop to monitor socket events.
 * 
 * This function runs a loop that calls the poll system call to monitor socket events.
 * It handles server and client events based on the poll results.
 * 
 * @param manager Reference to the SocketManager instance.
 * @throws std::runtime_error If an error occurs during the poll system call.
 */
void	SocketPollManager::runPoll(SocketManager& manager)
{
	int				ret;

	cout << "Running webserv, press ctrl + C or ctrl + \\ to end" << endl;
	while (true)
	{
		ret = poll(&_poll_fds->at(0), _poll_fds->size(), 60 * 1000); // one min
		if (ret < 0)
			throw runtime_error(string("SocketPollManager::runPoll(): ") + strerror(errno));
		if (ret == 0)
			continue ;

		for (size_t i = 0; i < _poll_fds->size() && ret; i++)
		{
			if (_poll_fds->at(i).revents == 0)
				continue ;
			SocketPollInfo socket_info = getSocketInfo(_poll_fds->at(i));
			if (socket_info.type == SERVER_SOCKET)
				serverHandler(socket_info, manager);
			else
				clientHandler(socket_info, manager, _poll_fds->at(i).events);
			ret--;
		}
	}
}
