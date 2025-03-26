/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:47:24 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/26 15:23:34 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "Config.hpp"

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
 * @brief Structure to hold information about a socket, its poll status and port.
 * @param port The port on which the socket is connected.
 * @param type Type of socket.
 * @param pfd pollfd structure containing informations about a polling request.
 */
struct SocketPollInfo
{
	int				port;
	e_SocketType	type;
	pollfd 			pfd;
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
 * @param size Size of the address strucutre.
 */
struct ClientInfo
{
	t_sockfd	client_fd;
	sockaddr_in	address;
	socklen_t	size;
	ClientInfo() : client_fd(-1), size(0)
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
 * @param server_fd The socket file descriptor for the server.
 * @param server_address The address information for the server.
 * @param clients A vector of ClientInfo structures.
 * @param server A pointer to a ServerConfig structure associated with the port.
 */
struct PortInfo
{
	t_sockfd						server_fd;
	sockaddr_in						server_address;
	vector<ClientInfo>				clients;
	vector<ServerConfig>			servers;
	PortInfo() : server_fd(-1)
	{
		memset(&server_address, 0, sizeof(server_address));
	}
};