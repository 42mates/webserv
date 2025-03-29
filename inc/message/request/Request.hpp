/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/29 17:43:32 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "Config.hpp"

#include "Response.hpp"

/*
 * @brief Represents an HTTP request.
 */
class Request
{
	private:
		string	_method;
		string	_uri;
		string	_version;
		map<string, string>	_header;
		string	_query;
		string	_body;
		string	_path;
		size_t	_body_size;
		bool	_is_complete_request;
		
		// INITIALIZATION METHODS
		void	initHeaderFields();
		void	initMethodHandling();
		
		// PARSING
		string	_raw_request;
		string	_raw_body;
		size_t	_start;
		bool	_header_parsed;
		string	_body_filename;
		fstream _body_stream;
		bool	isCompleteHeader(string raw_request);
		void	parseStartLine(string start_line);
		void	parseHeaderLine(string header_line);
		void	parseHeader(string header_line);
		void	decodeChunked(string body);
		void	parseBody(string body);

		// CHECKS
		void	checkStartLine();
		void	checkMethod();
		void	checkHeader();
		string	getFilePath(const string &path);

		// CGI
		char**		initEnv();
		Response	handle_cgi();
		
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
		virtual ~Request();

		void		parseRequest(string raw_request);
		Response	handleRequest(ServerConfig &server_config);

		// GETTERS
		size_t		getBodySize();
		string		getBodyString();
		string		getHeaderValue(string value);
		string		getConnectionKeepAlive();
		string		getRawRequest();
		bool		getIsCompleteRequest();

		// STATUS
		void		setIsCompleteRequest();
		void		setIsCompleteRequest(bool val);
		
		// TESTING
		void		printStartLine();
		void		printHeader();
		void		printBody();
		static void	printBody(string &body);
		void		print();
		void		printRaw();
		void		printRaw(string str);
};
