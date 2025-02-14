/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/14 13:53:40 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServ::WebServ()
{}

WebServ::~WebServ()
{}

void WebServ::run(const char* arg, int &ret)
{
	string _config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;

	try
	{
		_conf.parse(_config_file);

		SocketManager	sockets(_conf.getServers());
		_sockets = &sockets;
		sockets.runPollManager();
		// create and launch sockets

		// main loop
			// watch and accept connections
			// read from sockets
			// handle requests
			// send responses

		// close sockets	

	}
	catch(const exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
	}

}
