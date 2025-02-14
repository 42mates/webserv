/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:40:43 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/14 14:38:52 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class SocketManager;

class SocketPollManager
{
private:
		map<int, PortInfo>				*_ports_info;
		vector<pollfd>					*_poll_fds;
		map<t_sockfd, SocketPollInfo>	*_socket_to_poll;

		Request							_request;
		Response						_response;
public:

		SocketPollManager(SocketManager& manager);
		~SocketPollManager( void );

		void	runPoll(SocketManager& manager);
	
		void	serverHandler(SocketPollInfo poll_info, SocketManager& manager);
		void	establishConnection(t_sockfd server_socket, int port, SocketManager& manager);

		void	clientHandler(SocketPollInfo poll_info, SocketManager& manager);
		ssize_t	clientSend(SocketPollInfo poll_info);
		void	clientRecv(SocketPollInfo poll_info);
	
	//! testing purposes

	friend ostream& operator<<(std::ostream& os, const SocketPollManager& spm);
};