/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/13 20:13:41 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Config.hpp"
# include "SocketOperations.hpp"
# include "SocketPollManager.hpp"

//! Should multiple servers listen on the same port ?
//! CAREFUL : Since _poll_manager need access to data stored in SocketManager, said data need
//! to be initialized first of all. Should be ok since c++ initiliaze data in the order they appear
//! in the class, but it still needs testing

class SocketManager 
{
	private:
		map<int, PortInfo>					_ports_info;
		vector<pollfd>						_poll_fds;
		map<t_sockfd, SocketPollInfo>		_socket_to_poll;

		SocketOperations					_ops;
		SocketPollManager					_poll_manager;


	public:
		SocketManager(const vector <ServerConfig*>* servers);	 
		~SocketManager();

							/*GETTERS*/
								
		map<t_sockfd, PortInfo>*		getPortsInfo( void );
		PortInfo*						getPortInfo(int port);
		vector<ClientInfo>*				getClientsInfo(int port);
		vector<pollfd>*					getPollFds( void );
		map<t_sockfd, SocketPollInfo>*	getSocketToPoll( void );

							/*UTILS*/
		void			storeSocket(int port, t_sockfd socket, short options, e_SocketType type, ClientInfo *client);
		void			removeClientSocket(int port, t_sockfd socket);
		void			removeServerSocket(int port);
		void			closeConnection(int port, t_sockfd socket, e_SocketType type);
		//? Client's socket in non blocking ? (i.e. should it wait for an answer?)

							/*TESTING*/

		void			printInfo( void ) const;
};	

ostream&	operator<<(ostream& o, PortInfo& rhs); //! testing purposes only


