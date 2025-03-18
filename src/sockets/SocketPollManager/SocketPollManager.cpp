/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:52:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/18 15:43:38 by mbecker          ###   ########.fr       */
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
	_socket_to_request.erase(socket_fd);
}


void	SocketPollManager::prepareData(t_sockfd socket_fd, ssize_t& total_bytes_read, Request& request)
{
	map<t_sockfd, Request>::iterator it = _socket_to_request.find(socket_fd);

	if (it == _socket_to_request.end())
		return ;
	total_bytes_read = it->second.getRawRequest().size();
	request = it->second;
}

