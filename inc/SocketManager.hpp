/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:53:28 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/10 15:38:50 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_MANAGER_HPP
# define SOCKET_MANAGER_HPP

# include "webserv.h"


/**
 * @brief 
 * @param ports
 * @param sockets
 * @param addresses
 */
class SocketManager 
{
	private:
		std::vector<int>				ports;
		std::vector<int>				sockets;
		std::vector<struct sockaddr_in> addresses;
	public:
		SocketManager(const std::vector<int>& ports);
		~SocketManager( void );
};

#endif