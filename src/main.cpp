/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:43:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 15:12:59 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.h"
#include "SocketManager.hpp"

int main()
{
	std::vector<int> ports;
	ports.push_back(80);
	ports.push_back(2001);
	ports.push_back(2002);
	std::vector<port_info>	p;
	SocketManager	sockets(ports);

	p = sockets.getPortsInfo();

	// for (size_t i = 0; i < ports.size(); i++)
	// {
	// 	std::cout
	// 	<< "socket fd : " << p[i].server_socket << "\n"
	// 	<< "port : " << p[i].port << "\n"
	// 	<< "IP family : " << p[i].server_address.sin_family << "\n"
	// 	<< "IP : " << p[i].server_address.sin_addr.s_addr << "\n"
	// 	<< "port (htons) : " << p[i].server_address.sin_port << "\n\n";
		
	// }
}