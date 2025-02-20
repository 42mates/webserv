/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/20 16:34:52 by sokaraku         ###   ########.fr       */
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
		
		// INITIALIZATION METHODS
		void	initHeaderFields();
		void	initMethodHandling();
		
		// PARSING
		size_t	_start;
		bool	_header_parsed;
		void	parseStartLine(string start_line);
		void	parseHeaderLine(string header_line);
		void	parseHeader(string header_line);
		void	parseBody(string body);
		
		// CHUNKED DECODING
		string	decodeChunked(string body);

		// CHECKS
		void	checkStartLine();
		void	checkHeader();
		
		// METHOD HANDLING
		map<string, Response (Request::*)()> _method_handling; // Map of methods to their handling functions
		ServerConfig	_server_conf;
		RouteConfig		_route_conf;
		Response	handleGet();
		Response	handleHead();
		Response	handlePost();
		Response	handleDelete();

	public:
		Request();

		void		parseRequest(string raw_request);
		Response	handleRequest(ServerConfig &server_config);

		string		getConnectionKeepAlive() { return (_header["connection"]); }
		
		void		testParsing(); // For debugging purposes
};
