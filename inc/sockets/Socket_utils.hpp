/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:48:01 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/06 17:18:34 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libs.h"

/**
 * @brief Type definition for a socket file descriptor.
 */
typedef int t_sockfd;

/**
 * @brief Enum representing the type of socket.
 */
enum e_SocketType
{
    SERVER_SOCKET,
    CLIENT_SOCKET
};

/**
 * @brief Structure to hold information about a socket and its poll status.
 */
struct SocketPollInfo
{
    e_SocketType type;
    pollfd pfd;
};


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
	socklen_t	size;

	ClientInfo() : client(-1), size(-1);
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

	PortInfo() :	server(-1)
	{
		memset(&server_address, 0, sizeof(server_address));
	}
};