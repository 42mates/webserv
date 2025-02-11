/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/17 09:50:59 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "libs.h"

class SocketManager 
{
	private:
		vector<struct PortInfo>		_ports_info;

			/*Individual manager. If ports are added dynamically
				might need to make them public*/
		void			createSocket(int index, int port_at_index);
		void			bindSocket(int index);
		void			listenSocket(int index);

	public:
		SocketManager(const vector<int>& ports);
		~SocketManager( void );

								/*GETERS*/
		vector<struct PortInfo>		getPortsInfo( void ) const;
};	

struct PortInfo
{
	int								port;
	int								server_socket;
	struct sockaddr_in				server_address;
	vector<int>						client_socket;
	vector<struct sockaddr_in>		client_address;
};
