/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/03 15:59:14 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/*



*/

//? Can there be multiple ServerConfig on one given port ?
/**
 * @brief Generates a server socket and its informations for each ports given as a parameter.
 */
SocketManager::SocketManager(const vector <ServerConfig*>* servers)
{
	for (size_t i = 0; i < servers->size(); i++)
	{
		try
		{
			int port = servers->at(i)->port;
			createSocket(port);
			setReusability(_ports_info[port].server_socket);
			setToNonBlockingMode(_ports_info[port].server_socket);
			bindSocket(port);
			listenSocket(port);
		}
		catch(exception& e) { cout << e.what() << endl; }
	}
}

/**
 * @brief Ensures that all client and server sockets ar closed for each port 
 * managed by the SocketManager.
 */
SocketManager::~SocketManager(void)
{
	for (map<int, PortInfo>::iterator port_it = _ports_info.begin(); port_it != _ports_info.end(); port_it++)
	{
		map<int, ClientInfo>	&a_clients = port_it->second.clients;
		for (map<int, ClientInfo>::iterator client_it = a_clients.begin(); client_it != a_clients.end(); client_it++)
		{
			if (client_it->second.socket != -1)
				close(client_it->second.socket);
		}
		if (port_it->second.server_socket != -1)
			close(port_it->second.server_socket);
	}
}

				//! testing purposes omly
ostream& operator<<(ostream& o, const PortInfo& rhs) 
{
    o << "Server Socket: " << rhs.server_socket << "\n";
    o << "Server Address: " << inet_ntoa(rhs.server_address.sin_addr) << ":" << ntohs(rhs.server_address.sin_port) << "\n";
    o << "Client Sockets: ";
    for (map<int, ClientInfo>::const_iterator it = rhs.clients.begin(); it != rhs.clients.end(); ++it) {
        o << it->first << " (" << inet_ntoa(it->second.address.sin_addr) << ":" << ntohs(it->second.address.sin_port) << ") ";
    }
    o << "\n";
    return o;
}
