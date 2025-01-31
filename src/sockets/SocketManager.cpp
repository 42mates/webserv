/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/31 09:48:50 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


//? Can there be multiple ServerConfing on one given port ?
/**
 * @brief Generates a server socket and its informations for each ports given as a parameter.
 */
SocketManager::SocketManager(const vector <ServerConfig*>* servers)
{
	for (size_t i = 0; i < servers->size(); i++)
	{
		try
		{
			createSocket(i, servers->at(i)->port);
			bindSocket(i);
			listenSocket(i);
			setToNonBlockingMode(_ports_info.at(i).server_socket);
		}
		catch(exception& e)
		{
			cout << e.what() << endl;
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
		vector<int>& r_clients = _ports_info[i].client_socket;
		for (size_t j = 0; j < r_clients.size(); j++)
		{
			if (r_clients[j] != -1) 
				close(r_clients.at(j));
		}
		if (_ports_info[i].server_socket != -1)
			close(_ports_info[i].server_socket);
	}
}

				//! testing purposes omly
ostream& operator<<(ostream& o, const PortInfo& rhs) 
{
    o << "Port: " << rhs.port << "\n";
    o << "Server Socket: " << rhs.server_socket << "\n";
    o << "Server Address: " << inet_ntoa(rhs.server_address.sin_addr) << ":" << ntohs(rhs.server_address.sin_port) << "\n";
    o << "Client Sockets: ";
    for (size_t i = 0; i < rhs.client_socket.size(); ++i) {
        o << rhs.client_socket[i] << " ";
    }
    o << "\n";
    return o;
}
