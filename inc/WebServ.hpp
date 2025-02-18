/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/18 14:42:37 by sokaraku         ###   ########.fr       */
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
		Config _conf;
		SocketManager	*_sockets;

		Response manageRequest(string raw_request, ServerConfig &server_config);

	public:
		WebServ();
		~WebServ();

		void run(const char* conf_file, int &ret);
};
