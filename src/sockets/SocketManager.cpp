/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/04 17:14:50 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"


/*

mapping socket fd to a given nfds structure
	

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
			createSocket(servers->at(i)->host, port);
			setReusability(_ports_info[port].server);
			setToNonBlockingMode(_ports_info[port].server);
			bindSocket(port);
			listenSocket(port);
			addSocketToPoll(_ports_info[port].server, POLLIN);
			
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
		vector<ClientInfo>&	a_client = port_it->second.clients; 
		for (size_t i = 0; i  < a_client.size(); i++)
		{
			if (a_client[i].client != -1)
				close(a_client[i].client);
		}
		if (port_it->second.server != -1)
			close(port_it->second.server);
	}
}

				//! testing purposes omly
ostream& operator<<(ostream& o, const PortInfo& rhs) 
{
    o << "Server Socket: " << rhs.server << "\n";
    o << "Server Address: " << inet_ntoa(rhs.server_address.sin_addr) << ":" << ntohs(rhs.server_address.sin_port) << "\n";
    o << "Client Sockets: ";
    for (vector<ClientInfo>::const_iterator it = rhs.clients.begin(); it != rhs.clients.end(); ++it) {
        o << it->client << " (" << inet_ntoa(it->address.sin_addr) << ":" << ntohs(it->address.sin_port) << ") ";
    }
    o << "\n";
    return o;
}
