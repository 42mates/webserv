/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:52:39 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/13 20:11:24 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SocketPollManager.hpp"
# include "SocketManager.hpp"

SocketPollManager::SocketPollManager(SocketManager& manager)
{
	_ports_info = manager.getPortsInfo();
	_poll_fds = manager.getPollFds();
	_socket_to_poll = manager.getSocketToPoll();
}

SocketPollManager::~SocketPollManager() {}


void	SocketPollManager::runPoll(SocketManager& manager)
{
	int				ret;
	int				curr_fd;
	int				curr_revents;
	e_SocketType	curr_type;

	while (true)
	{
		ret = poll(&_poll_fds->at(0), _poll_fds->size(), 60 * 1000); // one min
		if (ret < 0)
			throw runtime_error(string("SocketPollManager: runPoll()") + strerror(errno));
		if (ret == 0)
			continue ;
		for (size_t i = 0; i < _poll_fds->size() && ret; i++)
		{
			if (_poll_fds->at(i).revents == 0)
				continue ;
			
			curr_fd = _poll_fds->at(i).fd;
			curr_type = _socket_to_poll->at(curr_fd).type;
			curr_revents = _poll_fds->at(i).revents;

			if (curr_type == SERVER_SOCKET)
				serverHandler(_socket_to_poll->at(curr_fd), manager);
			else
				clientHandler(_socket_to_poll->at(curr_fd), manager);
			ret--;
		}
	}
}


// static bool operator==(const ClientInfo& c, t_sockfd socket) { return c.client == socket; }
// static bool operator==(const pollfd& s, t_sockfd socket) { return s.fd == socket; }