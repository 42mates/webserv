/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/27 16:24:03 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

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
	cout << response.getStatus() << " " << response.getReason() << endl;
	return response;
}

void WebServ::run(const char* arg, int &ret)
{
	string _config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;

	try
	{
		_conf.parse(_config_file);

		// create and launch sockets

		// main loop
			// watch and accept connections
			// read from sockets
			
			// handle requests
			manageRequest(getTestRequest(), *_conf.getServers()[0]);
			
			// send responses

		// close sockets

	}
	catch(const exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
	}

}
