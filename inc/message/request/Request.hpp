/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/12 16:20:17 by mbecker          ###   ########.fr       */
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
		string _method;
		string _uri;
		string _version;
		
		// INITIALIZATION METHODS
		void initHeaderFields();
		void initMethodHandling();
		
		// PARSING METHODS
		void parseStartLine(string start_line);
		void parseHeaderLine(string header_line);
		void parseBody(string body);
		
		// CHUNKED DECODING
		string decodeChunked(string body);

		// CHECKS
		void checkStartLine();
		void checkHeader();
		void makeChecks();
		
		// METHOD HANDLING
		map<string, Response (Request::*)()> _method_handling; // Map of methods to their handling functions
		ServerConfig _server_conf;
		RouteConfig _route_conf;
		Response handleGet();
		Response handlePost();
		Response handleDelete();

	public:
		Request();

		void parseRequest(string raw_request);
		Response handleRequest(ServerConfig &server_config);
		void test();
};
