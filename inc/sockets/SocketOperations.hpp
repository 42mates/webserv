/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketOperations.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:48:13 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/11 15:58:49 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Socket.hpp"

class SocketOperations
{
public:

	SocketOperations( void ) {};
	~SocketOperations( void ) {};

    void createSocket(const std::string& ip, int port, PortInfo& portInfo);
    void bindSocket(PortInfo& portInfo);
    void listenSocket(PortInfo& portInfo);
    void setToNonBlockingMode(t_sockfd socket);
    void setReusability(t_sockfd socket);
    void closeSocket(t_sockfd socket);
};