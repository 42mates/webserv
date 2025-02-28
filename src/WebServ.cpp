/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 15:47:24 by sokaraku         ###   ########.fr       */
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

void	WebServ::setVariables(SocketManager& manager)
{
	SocketPollManager&	poll_manager = manager.getSocketPollManager();

	_poll_fds = manager.getPollFds();
	_socket_to_request = poll_manager.getSocketToRequest();
}

int WebServ::checkForEvents()
{
	int	timeout = 60 * 1000;
	return poll(&_poll_fds->at(0), _poll_fds->size(), timeout);
}

void WebServ::handleAllEvents(int events, SocketManager& manager)
{
	for (size_t i = 0; i < _poll_fds->size() && events; i++)
	{
		if (_poll_fds->at(i).revents == 0)
			continue ;
		handleOneEvent(_poll_fds->at(i), manager);
		events--;
	}
}

void WebServ::handleOneEvent(pollfd& poll_fd, SocketManager& manager)
{
	static SocketPollManager& poll_manager = manager.getSocketPollManager();
	SocketPollInfo	poll_info = poll_manager.getSocketInfo(poll_fd);

	try
	{
		if (poll_info.type == SERVER_SOCKET)
			poll_manager.serverHandler(poll_info, manager);
		else
			poll_manager.clientHandler(poll_info, manager, poll_fd.events);
	}
	catch (ResponseException& e)
	{
		cerr << "debug: " << e.what() << endl; //debug message 
		
		Response response = e.getResponse();
		(*_socket_to_response)[poll_fd.fd] = response;
	}
	
	
}

void WebServ::run(const char* arg, int& ret)
{
	string config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;
	SocketManager	*sockets = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	try
	{
		_conf.parse(config_file);
		sockets = new SocketManager(_conf.getServers());
		setVariables(*sockets);
		while (run_server)
		{
			int events = checkForEvents();
			if (events <= 0)
				continue ;
			handleAllEvents(events, *sockets);
		}
	}
	catch (exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
	}
	if (sockets != NULL)
		delete sockets;
}