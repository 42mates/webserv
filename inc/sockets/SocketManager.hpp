/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/30 17:09:15 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "libs.h"
# include "Config.hpp"

struct PortInfo
{
	int								port;
	int								server_socket;
	struct sockaddr_in				server_address;
	vector<int>						client_socket;
	vector<struct sockaddr_in>		client_address;
};

class SocketManager 
{
	private:
		vector<PortInfo>		_ports_info;
		
		void			createSocket(int index, int port_at_index);
		void			bindSocket(int index);
		void			listenSocket(int index);

	public:
		SocketManager(const vector <ServerConfig*>* servers);
		~SocketManager( void );

								/*GETERS*/
		vector<PortInfo>*		getPortsInfo( void );
		PortInfo*				getPortInfo(int index);
};	

ostream&	operator<<(ostream& o, PortInfo& rhs); //! testing purposes only


