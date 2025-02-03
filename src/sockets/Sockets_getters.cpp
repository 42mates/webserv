/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets_getters.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:34:48 by sokaraku          #+#    #+#             */
/*   Updated: 2025/02/03 15:09:35 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"

map<int, PortInfo>*	SocketManager::getPortsInfo(void) { return &_ports_info; }

PortInfo*	SocketManager::getPortInfo(int index)
{
	if (index > static_cast<int>(_ports_info.size()))
	{
		cerr << "port informations at index [" << index << "] not found" << endl;
		return NULL;
	}
	return &_ports_info.at(index);
}
