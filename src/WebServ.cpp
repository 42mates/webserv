/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 13:22:45 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

volatile int run_server = true;
void	handle_sigint(int signum) { (void)signum, run_server = false; }
void	handle_sigquit(int signum) { (void)signum, run_server = false; }


WebServ::WebServ()
{}

WebServ::~WebServ()
{}


void WebServ::handleAction()
{
	//handleAction
	try
	{
		//gestion des requests
	}
	catch (ResponseException& e)
	{
		cerr << "debug: " << e.what() << endl; //debug message 
		
		Response response = e.getResponse();
	}
	//envoi des reponses
}


//100 continue 
//todo implement a getsockopt for error retrieving and error message handling
void WebServ::run(const char* arg, int &ret)
{
	string _config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;
	SocketManager	*sockets = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	try
	{
		_conf.parse(_config_file);

		sockets = new SocketManager(_conf.getServers());

		while (run_server)
			sockets->runPollManager();
	
		//checkPoll

		handleAction();

	}
	catch(const exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
	}
	if (sockets != NULL)
		delete sockets;
	
}
