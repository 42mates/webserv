/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/06 21:00:46 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "libs.h" //to remove if Socket_utils is included
# include "Config.hpp"
// # include "Socket_utils.hpp"


 
/**
 * @brief Type definition for a socket file descriptor.
 */
typedef int t_sockfd;

/**
 * @brief Enum representing the type of socket.
 * 
 * @param SERVER_SOCKET
 * @param CLIENT_SOCKET 
 */
enum e_SocketType
{
    SERVER_SOCKET,
    CLIENT_SOCKET
};

/**
 * @brief Structure to hold information about a socket and its poll status.
 * @param type Type of socket.
 * @param pfd pollfd structure containing informations about a polling request.
 */
struct SocketPollInfo
{

    e_SocketType	type;
    pollfd 			pfd;
};
//? storing he port on which the socket is ?

/**
 * @brief Stores information about a connected client, for a given port.
 * 
 * This structure holds the socket file descriptor and address information
 * for a connected client. It is used to manage client connections in the
 * SocketManager class.
 * 
 * @param client The file descriptor for the client socket. 
 * @param address Address information for the client.
 * @param size Size of the address strucutre.
 */
struct ClientInfo
{
	t_sockfd	client;
	sockaddr_in	address;
	socklen_t	size;

	ClientInfo() : client(-1), size(0)
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
 * @param clients A vector of ClientInfo structures.
 */
struct PortInfo
{
	t_sockfd						server;
	sockaddr_in						server_address;
	vector<ClientInfo>				clients;

	PortInfo() : server(-1)
	{
		memset(&server_address, 0, sizeof(server_address));
	}
};

//! Should multiple servers listen on the same port ?

class SocketManager 
{
	private:
		map<int, PortInfo>					_ports_info;
		// vector<SocketPollInfo>				_poll_info;
		vector<pollfd>						_poll_fds;
		map<t_sockfd, SocketPollInfo>		_socket_to_poll;

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

							/*UTILS*/
							
		void			setToNonBlockingMode(t_sockfd socket); //ToDo set non blocking for server's socket and the accept() socket
		void			setReusability(t_sockfd socket);
		void			storeSocket(int port, t_sockfd socket, short options, e_SocketType type, ClientInfo *client);
		//? Client's socket in non blocking ? (i.e. should it wait for an answer?)

							/*OPERATIONS*/

		void	runPoll();
		void	establishConnection(t_sockfd server_socket, int port);
		void	handleClient(t_sockfd socket, int action); //see for storing it with |
		void	closeConnection(int port, t_sockfd socket);
		void	removeSocket(int port, t_sockfd socket);

		void			printInfo( void ) const;
};	

ostream&	operator<<(ostream& o, PortInfo& rhs); //! testing purposes only


