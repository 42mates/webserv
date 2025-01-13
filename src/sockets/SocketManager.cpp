/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/13 16:06:52 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

# define SOCKET_CREATION_ERROR "Error.\nUnable to create socket's fd."
# define SOCKET_BINDING_ERROR "Error.\nUnable to bind socket."
# define SOCKET_LISTENING_ERROR "Error.\nUnable to set one socket to a listening state."
/**
 * @brief Generates a socket and its informations for each ports given as a parameter.
 */
SocketManager::SocketManager(const std::vector<int>& ports)
{
	for (size_t i = 0; i < ports.size(); i++)
	{
		
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
			close(r_clients.at(j));
		close(_ports_info[i].server_socket);
	}
}

/**
 * @brief Creates a socket for the specified port index. 
 * @param index The index of the port to which the socket is to be created.
 */
void	SocketManager::createSocket(int index)
{
	std::vector<int>&			r_clients_fds = _ports_info[index].client_socket;
	std::vector<sockaddr_in>&	r_clients_addresses = _ports_info[index].client_address;

	int	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Address family (IPv4), type of socket, protocol (TCP). 
	if (socket_fd == -1)
		throw std::runtime_error(SOCKET_CREATION_ERROR);
	r_clients_fds.push_back(socket_fd);

	r_clients_addresses[index].sin_family = AF_INET;
	r_clients_addresses[index].sin_port = htons(_ports_info[index].port);
	r_clients_addresses[index].sin_addr.s_addr = INADDR_ANY; // COME BACK INADDR_ANY = server accessible from all interfaces
	
}

/**
 * @brief Binds the socket to the specified port index.
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
 * incoming connection requests, using accept().
 * @param index The index of the port for which the socket is to listen.
 */
void	SocketManager::listenSocket(int index)
{
	if (listen(_ports_info[index].server_socket, 1) < 0) // the second parameter will probably change
		throw std::runtime_error(SOCKET_LISTENING_ERROR);
}

std::vector<struct port_info>	SocketManager::getPortsInfo(void) const { return (_ports_info); }