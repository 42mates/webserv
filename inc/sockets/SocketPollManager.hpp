/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:40:43 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/16 20:53:43 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Socket.hpp"

#include "Request.hpp"
#include "Response.hpp"

class SocketManager;

/**
 * @brief Manages socket polling operations in a non-blocking manner.
 * 
 * The SocketPollManager class is responsible for managing socket polling operations
 * using the poll system call. It interacts with the SocketManager class to handle
 * various socket events, such as incoming connections, data readiness for reading
 * or writing, hang-ups, and errors.
 */
class SocketPollManager
{
private:
		map<int, PortInfo>				*_ports_info;
		vector<pollfd>					*_poll_fds;
		map<t_sockfd, SocketPollInfo>	*_socket_to_poll;

		map<t_sockfd, Request>			_socket_to_request;
		map<t_sockfd, Response>			_socket_to_response;

public:

		SocketPollManager(SocketManager& manager);
		~SocketPollManager( void );

		// void	runPoll(SocketManager& manager);
	
		void	serverHandler(SocketPollInfo poll_info, SocketManager& manager);
		void	establishConnection(t_sockfd server_socket, int port, SocketManager& manager);

		void	clientHandler(SocketPollInfo poll_info, SocketManager& manager, short& fd_events);
		ssize_t	clientSend(SocketPollInfo& poll_info, SocketManager& manager, ServerConfig& server);
		void	clientRecv(SocketPollInfo poll_info, ServerConfig& server);

		void	prepareData(t_sockfd socket_fd, ssize_t& total_bytes_read, Request& request);
		// void	prepareData(t_sockfd socket_fd, ssize_t& total_bytes_sent, Response& response);
		
		SocketPollInfo				getSocketInfo(pollfd pfd);
		map<t_sockfd, Request>*		getSocketToRequest( void );
		map<t_sockfd, Response>*	getSocketToResponse( void );

		void	removeSocket(t_sockfd socket_fd);
	//! testing purposes

	friend ostream& operator<<(std::ostream& os, const SocketPollManager& spm);
};


/**
 * @brief Struct to hold event handler information.
 * 
 * This struct contains the event bitmask and the corresponding handler function pointer.
 */
struct eventHandler
{
	short event;
	void (*handler)(SocketPollInfo, SocketManager&, SocketPollManager&);
};


int		checkSocketStatus(t_sockfd socket_fd);


void	serverPollIn(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);
void	serverPollHup(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);
void	serverPollErr(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);

void	clientPollIn(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);
void	clientPollOut(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);
void	clientPollHup(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);
void	clientPollErr(SocketPollInfo poll_info, SocketManager& manager, SocketPollManager& poll_manager);

