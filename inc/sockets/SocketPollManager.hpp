/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:40:43 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/13 20:37:42 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Socket.hpp"
# include "Request.hpp"

class SocketManager;

class SocketPollManager
{
private:
		map<int, PortInfo>				*_ports_info;
		vector<pollfd>					*_poll_fds;
		map<t_sockfd, SocketPollInfo>	*_socket_to_poll;

		Request							_request;
public:

		SocketPollManager(SocketManager& manager);
		~SocketPollManager( void );

		void	runPoll(SocketManager& manager);
		void	serverHandler(SocketPollInfo poll_info, SocketManager& manager);
		void	clientHandler(SocketPollInfo poll_info, SocketManager& manager);
		void	establishConnection(t_sockfd server_socket, int port, SocketManager& manager);
		void	actionHandler(t_sockfd socket, int revents, int type); //see for storing it with |
};