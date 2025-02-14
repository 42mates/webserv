/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:52:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/14 15:50:24 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPollManager.hpp"
#include "SocketManager.hpp"

SocketPollManager::SocketPollManager(SocketManager& manager) : _response("300")
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



void	SocketPollManager::runPoll(SocketManager& manager)
{
	int				ret;
	int				curr_fd;
	int				curr_revents;
	e_SocketType	curr_type;
	SocketPollInfo	tmp;

	cout << *this << endl;
	while (true)
	{
		// cout << "Waiting for incoming connections..." << endl; //!testing purpose
		ret = poll(&_poll_fds->at(0), _poll_fds->size(), 60 * 1000); // one min
		
		if (ret < 0)
			throw runtime_error(string("SocketPollManager: runPoll(): ") + strerror(errno));
		if (ret == 0)
			continue ;
		for (size_t i = 0; i < _poll_fds->size() && ret; i++)
		{
			if (_poll_fds->at(i).revents == 0)
				continue ;
			
			curr_fd = _poll_fds->at(i).fd;
			curr_type = _socket_to_poll->at(curr_fd).type;
			curr_revents = _poll_fds->at(i).revents;

			tmp.pfd = _poll_fds->at(i);
			tmp.port = _socket_to_poll->at(curr_fd).port;
			tmp.type = curr_type;
			if (curr_type == SERVER_SOCKET)
				serverHandler(tmp, manager);
			else
				clientHandler(tmp, manager);
			ret--;
		}
	}
}
