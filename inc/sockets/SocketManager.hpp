/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/03 15:59:37 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "libs.h"
# include "Config.hpp"


/**
 * @brief Stores information about a connected client.
 * 
 * This structure holds the socket file descriptor and address information
 * for a connected client. It is used to manage client connections in the
 * SocketManager class.
 * 
 * @param client_socket The file descriptor for the client socket. 
 * @param address Address information for the client.
 */
struct ClientInfo
{
	int			socket;
	sockaddr_in	address;

	ClientInfo() : socket(-1) {}
};

/**
 * @brief Stores information about a listening port.
 * 
 * This structure holds the server socket, server address, and a map of
 * connected clients for a specific port. It is used to manage the server
 * sockets and client connections in the SocketManager class.
 * 
 * @param server_socket The socket file descriptor for the server.
 * @param server_address The address information for the server.
 * @param clients A map of client sockets to ClientInfo structures.
 */
struct PortInfo
{
	int								server_socket;
	sockaddr_in						server_address;
	map<int, ClientInfo>			clients;

	PortInfo() :
		server_socket(-1)
		{
			memset(&server_address, 0, sizeof(server_address));
		}
};

class SocketManager 
{
	private:
		map<int, PortInfo>		_ports_info;
	
		void			createSocket(int port);
		void			bindSocket(int index);
		void			listenSocket(int index);

	public:
		SocketManager(const vector <ServerConfig*>* servers);
		~SocketManager();

								/*GETERS*/
		map<int, PortInfo>*		getPortsInfo( void );
		PortInfo*				getPortInfo(int index);

		void			setToNonBlockingMode(int socket_fd); //ToDo set non blocking for server's socket and the accept() socket
		void			setReusability(int socket);
		//? Client's socket in non blocking ? (i.e. should it wait for an answer?)
};	

ostream&	operator<<(ostream& o, PortInfo& rhs); //! testing purposes only


