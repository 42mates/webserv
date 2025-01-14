/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/14 16:17:45 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

/**
 * @brief Generates a server socket and its informations for each ports given as a parameter.
 */
SocketManager::SocketManager(const std::vector<int>& ports)
{
	for (size_t i = 0; i < ports.size(); i++)
	{
		try 
		{
			createSocket(i, ports.at(i));
			bindSocket(i);
			listenSocket(i);
		}
		catch(std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

/**
 * @brief Ensures that all client and server sockets ar closed for each port 
 * managed by the SocketManager.
 */
SocketManager::~SocketManager(void)
{
	for (size_t i = 0; i < _ports_info.size(); i++)
	{
		std::vector<int>& r_clients = _ports_info[i].client_socket;
		for (size_t j = 0; j < r_clients.size(); j++)
		{
			if (r_clients[j] != -1) 
				close(r_clients.at(j));
		}
		if (_ports_info[i].server_socket != -1)
			close(_ports_info[i].server_socket);
	}
}

/**
 * @brief Creates a socket for the specified port index. At this point, the socket
 * is just a file descriptor, and isn't bound to any IP address or port yet.
 * @param port_at_index The index of the port to which the socket is to be created.
 */
void	SocketManager::createSocket(int index, int port_at_index)
{
	if (static_cast<size_t>(index) >= _ports_info.size())
		_ports_info.resize((index + (index == 0)) * 2);

	struct sockaddr_in&	r_server_address = _ports_info[index].server_address;

	int	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Address family (IPv4), type of socket, protocol (TCP). 
	if (socket_fd == -1)
		throw std::runtime_error(SOCKET_CREATION_ERROR);
	
	_ports_info[index].server_socket = socket_fd;
	_ports_info[index].port = port_at_index;

	r_server_address.sin_family = AF_INET; //same as before, sets the address family as IPv4
	r_server_address.sin_port = htons(_ports_info[index].port); // sets the bits to big-endian (consistent with network protocols)
	r_server_address.sin_addr.s_addr = INADDR_ANY; // COME BACK INADDR_ANY = server accessible from all interfaces
}

/**
 * @brief Binds the socket to the specified port index and the associated IP address.
 * Now, the socket is addressable : clients can locate it with the assigned IP/port pair.
 * @param index index of the port to which the socket is to be bound.
 */
void	SocketManager::bindSocket(int index)
{
	sockaddr_in&	r_server_address = _ports_info[index].server_address;
	int&			r_server_socket = _ports_info[index].server_socket;

	if (bind(r_server_socket, (struct sockaddr*)(&r_server_address), sizeof(r_server_address)) < 0)
		throw std::runtime_error(SOCKET_BINDING_ERROR);
}

/**
 * @brief Sets a socket to a passive listening state, so that it can wait for 
 * incoming connection requests, using accept(). At this point, the socket can't send
 * or receive data directlt, it can just accept requests.
 * @param index The index of the port for which the socket is to listen.
 */
void	SocketManager::listenSocket(int index)
{
	//second parameter is basically a connexion queue, it sets the the maximum number
	// of connection requests that can be queued
	if (listen(_ports_info[index].server_socket, 1) < 0) // the second parameter will probably change
		throw std::runtime_error(SOCKET_LISTENING_ERROR);
}

std::vector<struct port_info>	SocketManager::getPortsInfo(void) const { return (_ports_info); }