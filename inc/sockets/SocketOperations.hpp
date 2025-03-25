/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketOperations.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:48:13 by sokaraku          #+#    #+#             */
/*   Updated: 2025/03/25 15:26:23 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

/**
 * @brief Provides various socket operations.
 * 
 * The SocketOperations class provides a set of methods to perform various
 * operations on sockets, such as creating, binding, listening, setting
 * non-blocking mode, setting reusability, and closing sockets.
 */
class SocketOperations
{
public:

	SocketOperations( void ) {};
	~SocketOperations( void ) {};

    void createSocket(const std::string& ip, int port, PortInfo& portInfo);
    void bindSocket(PortInfo& portInfo, int port);
    void listenSocket(PortInfo& portInfo);
    void setOptions(t_sockfd socket);
    void setReusability(t_sockfd socket);
    void closeSocket(t_sockfd socket); //! might need to remove that
};