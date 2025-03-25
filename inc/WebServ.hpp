/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/25 15:35:57 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"
#include "Config.hpp"
#include "Request.hpp"
#include "SocketManager.hpp"

class WebServ
{
	private:
		Config 						_conf;
		vector<pollfd>* 			_poll_fds;
		map<t_sockfd, Request>*		_socket_to_request;
		// map<t_sockfd, Response>*	_socket_to_response; //?useless
		
		void	setVariables(SocketManager& manager);
		int		checkForEvents();
		void	handleAllEvents(int events, SocketManager& manager);
		void 	handleOneEvent(pollfd& poll_fd, SocketManager& manager);

	public:
		WebServ();
		~WebServ();

		void 	run(const char* conf_file, int &ret);
};
