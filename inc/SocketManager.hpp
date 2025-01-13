/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/13 16:11:38 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_MANAGER_HPP
# define SOCKET_MANAGER_HPP

# include "webserv.h"

class SocketManager 
{
	private:
		std::vector<struct port_info>		_ports_info;

			/*Individual manager. If ports are added dynamically
				might need to make them public*/
		void			createSocket(int index);
		void			bindSocket(int index);
		void			listenSocket(int index);

	public:
		SocketManager(const std::vector<int>& ports);
		~SocketManager( void );

								/*GETERS*/
		std::vector<struct port_info>		getPortsInfo( void ) const;
};	

struct port_info
{
	int								port;
	int								server_socket;
	struct sockaddr_in				server_address;
	std::vector<int>				client_socket;
	std::vector<struct sockaddr_in>	client_address;
};

#endif