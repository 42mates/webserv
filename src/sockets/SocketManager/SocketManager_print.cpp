/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager_print.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:31:02 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/06 20:00:50 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"
#include "macros.h"

void SocketManager::printInfo() const
{
    cout << "SocketManager Information:" << endl;
    cout << "---------------------------" << endl;

    if (_ports_info.empty())
    {
        cout << "No port information available." << endl;
    }
    else
    {
        for (map<int, PortInfo>::const_iterator it = _ports_info.begin(); it != _ports_info.end(); it++)
        {
            cout << "Port: " << it->first << endl;
            cout << "Server Socket: " << it->second.server << endl;
            cout << "Server Address: " << inet_ntoa(it->second.server_address.sin_addr) << ":" << ntohs(it->second.server_address.sin_port) << endl;
            cout << "Client Sockets: ";
            if (it->second.clients.empty())
            {
                cout << "None";
            }
            else
            {
                for (vector<ClientInfo>::const_iterator client_it = it->second.clients.begin(); client_it != it->second.clients.end(); client_it++)
                {
                    cout << client_it->client << " (" << inet_ntoa(client_it->address.sin_addr) << ":" << ntohs(client_it->address.sin_port) << ") ";
                }
            }
            cout << endl;
        }
    }

    cout << "Poll FDs:" << endl;
    if (_poll_fds.empty())
    {
        cout << "No poll file descriptors available." << endl;
    }
    else
    {
        for (vector<pollfd>::const_iterator poll_it = _poll_fds.begin(); poll_it != _poll_fds.end(); poll_it++)
        {
            cout << "FD: " << poll_it->fd << ", Events: " << poll_it->events << ", Revents: " << poll_it->revents << ", Type: " << endl;
        }
    }

    cout << "Socket to Poll Mapping:" << endl;
    if (_socket_to_poll.empty())
    {
        cout << "No socket to poll mapping available." << endl;
    }
    else
    {
        for (map<t_sockfd, SocketPollInfo>::const_iterator map_it = _socket_to_poll.begin(); map_it != _socket_to_poll.end(); map_it++)
        {
            cout << "Socket FD: " << map_it->first << ", Poll FD: " << map_it->second.pfd.fd << ", Poll Events: " << map_it->second.pfd.events << ", Poll Revents: " << map_it->second.pfd.revents << ", Type: " << (map_it->second.type == SERVER_SOCKET ? "Server" : "Client") << endl;
        }
    }
}