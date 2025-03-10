/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/10 15:19:18 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "Config.hpp"

#include "AMessage.hpp"
#include "Response.hpp"

/*
 * @brief Represents an HTTP request.
 */
class Request : public AMessage
{
	private:
		string	_method;
		string	_uri;
		string	_version;
		string	_path;
		
		// INITIALIZATION METHODS
		void	initHeaderFields();
		void	initMethodHandling();
		
		// PARSING
		string	_raw_request;
		bool	_is_complete_request;
		bool	_header_parsed;
		size_t	_start;
		void	parseStartLine(string start_line);
		void	parseHeaderLine(string header_line);
		void	parseHeader(string header_line);
		void	parseBody(string body);
		
		//STATUS
		bool	isCompleteHeader(string raw_request);
		
		// CHUNKED DECODING
		bool	_chunked_complete;
		string	decodeChunked(string body);

		// CHECKS
		void	checkStartLine();
		void	checkHeader();
		string	getFilePath(const string &path);
		void	checkMethod();
		
		// METHOD HANDLING
		map<string, Response (Request::*)()> _method_handling; // Map of methods to their handling functions
		ServerConfig	_server_conf;
		RouteConfig		_route_conf;
		string		getFileContent();
		Response	handleGET();
		Response	handleHEAD();
		Response	handlePOST();
		Response	handleDELETE();

	public:
		Request();

		void		parseRequest(string raw_request);
		Response	handleRequest(ServerConfig &server_config);

		// GETTERS
		string		getHeaderValue(string value);
		string		getConnectionKeepAlive();

		// STATUS
		void		setIsCompleteRequest();
		
		// TESTING
		void		testParsing();
		void		printStartLine();
		void		printHeader();
		void		printBody();
		void		printBody(string &body);
		void		print();
		void		printRaw();
};
