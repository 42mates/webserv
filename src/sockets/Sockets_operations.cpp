/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_operations.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:35:36 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/31 10:16:05 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

void	SocketManager::setToNonBlockingMode(int socket_fd)
{
	int	flags = fcntl(socket_fd, F_GETFL, 0);

	if (flags == -1)
		throw runtime_error(string("SocketManager: ") + strerror(errno));

	if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) //*not using fcntl directly cause it'll overwrite all flags
		throw runtime_error(string("SocketManager: ") + strerror(errno));
	//! testing purposes
	cout << "Socket: " << socket_fd << " set to non blocking mode\n";
}