/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/20 14:11:51 by sokaraku         ###   ########.fr       */
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

/**
 * @brief Manages an incoming HTTP request and generates an appropriate response.
 *
 * @param raw_request The raw HTTP request string to be processed.
 * @param server_config The server configuration object used to handle the request.
 * @returns A reference to the generated Response object.
 */
Response WebServ::manageRequest(string raw_request, ServerConfig &server_config)
{
	Request request;
	Response response;

	try
	{
		request.parseRequest(raw_request);
		response = request.handleRequest(server_config);
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: manageRequest(): " << e.what() << endl;
		response = e.getResponse();
	}
	return response;
}





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
	}
	catch(const exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
		if (sockets != NULL)
			delete sockets;
		return ;
	}
	while (run_server)
	{
		try
		{
			sockets->runPollManager();
		}
		catch(const exception& e)
		{
			cerr << "webserv: " << e.what() << endl;
			ret = 1;
			delete sockets;
			return ;
		}
	}
	delete sockets;
	// try
	// {
	// 	_conf.parse(_config_file);
	// 	SocketManager	sockets(_conf.getServers());
	// 	_sockets = &sockets;
	// 	_sockets->runPollManager();
	// 	// create and launch sockets

	// 	// main loop
	// 		// watch and accept connections
	// 		// read from sockets
	// 		// Request request;
	// 		// request.testParsing();
	// 		// handle requests
	// 		// send responses

	// 	// close sockets

	// }
	// catch(const exception& e)
	// {
	// 	cerr << "webserv: " << e.what() << endl;
	// 	ret = 1;
	// }

}
