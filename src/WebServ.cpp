/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 17:09:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

volatile int run_server = true;
void	handle_sigint(int signum)
{
	(void)signum;
	run_server = false;
	cout << endl;
}
void	handle_sigquit(int signum)
{
	(void)signum;
	run_server = false; 
	cout << endl;
}


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
	if (_poll_fds->empty())
		throw runtime_error("no socket to monitor");
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
	catch(exception& e) 
	{ 
		error_log << "handleOneEvent(): " << e.what() << endl; 
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
		//printConfig(*_conf.getBestServer("0.0.0.0", 42420, "localhost")); //debug
		sockets = new SocketManager(_conf.getServers());
		setVariables(*sockets);
		while (run_server)
		{
			int events = checkForEvents();
			if (events == 0)
				continue ;
			else if (events < 0)
				throw runtime_error(strerror(errno));
			handleAllEvents(events, *sockets);
		}
	}
	catch (const exception& e)
	{
		error_log << "webserv: " << e.what() << endl;
		ret = 1;
	}
	if (sockets != NULL)
		delete sockets;
}