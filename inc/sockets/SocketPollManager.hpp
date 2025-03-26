/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPollManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:40:43 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/26 15:59:17 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Socket.hpp"

#include "Request.hpp"
#include "Response.hpp"

class SocketManager;
struct infoResponse;

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
		map<t_sockfd, infoResponse>		_socket_to_response;

public:

		SocketPollManager(SocketManager& manager);
		~SocketPollManager( void );
	
								/* SERVER SIDE */

		void	serverHandler(SocketPollInfo poll_info, SocketManager& manager);
		void	establishConnection(t_sockfd server_socket, int port, SocketManager& manager);
		void	closeConnectionToAllClients(Response& r, SocketPollInfo server, SocketManager& manager);

								/* CLIENT SIDE */
				
		void	clientHandler(SocketPollInfo poll_info, SocketManager& manager, short& fd_events);
		

		
		ssize_t	clientSend(SocketPollInfo& poll_info, Response& response);
		ssize_t	clientSend(SocketPollInfo& poll_info, SocketManager& manager, ServerConfig& server);
		
		void	clientRecv(SocketPollInfo poll_info, ServerConfig& server);
		
												//--> utils
		
		void	prepareRecv(t_sockfd socket_fd, size_t& total_bytes_read, Request& request, timeval& start);
		void	prepareSend(t_sockfd socket_fd, size_t& len_sent, Response& response, timeval& start, vector<ServerConfig>& servers);
		
		ssize_t	readOneChunk(t_sockfd socket_fd, string& raw_request, size_t client_max_body_size, size_t& total_bytes_read, int& status);
		
		void	checkRequestTimeout(timeval& start, timeval& end);
		void	checkResponseTimeout(timeval& start, timeval& end);

		void	recvError(t_sockfd socket_fd, int& status, Request& request);
		void	sendError(t_sockfd socket_fd, int& status, Response& response, size_t& len_sent);
		
		bool	isMessageEnd(t_sockfd socket_fd, __int8_t type);
		
		
		

		

								/* GETTERS */
		
		SocketPollInfo					getSocketInfo(pollfd pfd);
		map<t_sockfd, Request>*			getSocketToRequest( void );
		map<t_sockfd, infoResponse>*	getSocketToResponse( void ); 


								/* OTHER */

		void	removeSocket(t_sockfd socket_fd);
};

bool	keepConnectionOpen(Response& r);

/**
 * @brief This structure holds a `Response` object and the length of the data it sent (`len_sent`) to the client.
 *
 */
struct infoResponse
{
	Response		response;
	size_t			len_sent;
	bool			fully_sent;
	ServerConfig	*best_server;

	infoResponse() : len_sent(0), fully_sent(false), best_server(NULL) {}
	infoResponse(const Response& r, const size_t& len) : response(r), len_sent(len), fully_sent(false) {}
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

