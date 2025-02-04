/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/04 17:50:29 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "libs.h"
# include "Config.hpp"

typedef int t_sockfd;

/**
 * @brief Stores information about a connected client, for a given port.
 * 
 * This structure holds the socket file descriptor and address information
 * for a connected client. It is used to manage client connections in the
 * SocketManager class.
 * 
 * @param client The file descriptor for the client socket. 
 * @param address Address information for the client.
 */
struct ClientInfo
{
	t_sockfd	client;
	sockaddr_in	address;

	ClientInfo() : client(-1)
	{
		memset(&address, 0, sizeof(address));
	}
};

/**
 * @brief Stores information about a listening port.
 * 
 * This structure holds the server socket, server address, and a map of
 * connected clients for a specific port. It is used to manage the server
 * sockets and client connections in the SocketManager class.
 * 
 * @param server The socket file descriptor for the server.
 * @param server_address The address information for the server.
 * @param clients A map of client sockets to ClientInfo structures.
 */
struct PortInfo
{
	t_sockfd						server;
	sockaddr_in						server_address;
	vector<ClientInfo>				clients;

	PortInfo() :	server(-1)
	{
		memset(&server_address, 0, sizeof(server_address));
	}
};

//! Should multiple servers listen on the same port ?

class SocketManager 
{
	private:
		map<int, PortInfo>					_ports_info;
		// map<int, vector<ClientInfo> >	_clients_info;
		vector<pollfd>						_poll_fds;
		map<t_sockfd, pollfd>				_socket_to_poll;

		void			createSocket(const string& ip, int port);
		void			bindSocket(int index);
		void			listenSocket(int index);

	public:
		SocketManager(const vector <ServerConfig*>* servers);
		~SocketManager();

								/*GETERS*/
								
		map<t_sockfd, PortInfo>*	getPortsInfo( void );
		PortInfo*					getPortInfo(int port);
		vector<ClientInfo>*			getClientsInfo(int port);
		pollfd*						getPollfd(t_sockfd socket);

		void			setToNonBlockingMode(t_sockfd socket); //ToDo set non blocking for server's socket and the accept() socket
		void			setReusability(t_sockfd socket);
		void			addSocketToPoll(t_sockfd socket, short options);
		//? Client's socket in non blocking ? (i.e. should it wait for an answer?)

		void			printInfo( void ) const;
};	

ostream&	operator<<(ostream& o, PortInfo& rhs); //! testing purposes only


