/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/24 12:49:19 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

#include "Config.hpp"

/**
 * @class Response
 * @brief Represents an HTTP response with status, headers, body, and debug information.
 */
class Response
{
	private:
		string _code;
		map<string, string> _header;
		string _body;

		map<string, string> _status_line; // status code to reason phrase
		void initStatusLine();

		void initHeaderFields();

		string	headerToString();
		string	addCRLF(string str);

		// PARSING
		size_t	_start;
		void	parseStartLine(string start_line);
		void	parseHeaderLine(string header_line);
		void	parseHeader(string header_line);
		void	parseBody(string body);

	public:
		Response();
		Response(string status);
		Response(const Response &other);
		Response &operator=(const Response &other);
		virtual ~Response();

		void	parseResponse(string raw_response);

		// SETTERS
		void	setCode(string status);
		void	setHeaderValue(string key, string value);
		void	setBody(string body);
		void	setErrorBody();
		void	setErrorBody(ServerConfig &server_conf, string &root);
		void	setDate();

		// GETTERS
		string	getResponse();
		string	getCode();
		string	getReason();
		string	getHeader();
		string	getHeader(string key);
		string	getBody();
		
		void print(); // debug
};

/**
 * @brief Exception class for throwing Response objects.
 */
class ResponseException : public exception
{
	private:
		Response _response;
		string _debug; // debug
	public:
		ResponseException(const Response &response) : _response(response) {}
		ResponseException(const Response &response, string debug) : _response(response), _debug(debug) {} // debug
		~ResponseException() throw() {}

		const char *what() const throw() { return _debug.c_str(); } // debug
		Response getResponse() const { return _response; }
};

/**
 * @brief Exception class for throwing A 100 Continue Response.
 */
class ContinueException : public exception
{
	private:
		Response _response;
		string _debug; // debug
	public:
		ContinueException() : _response(Response("100")) {}
		ContinueException(string debug) : _response(Response("100")), _debug(debug) {} // debug
		~ContinueException() throw() {}

		const char *what() const throw() { return _debug.c_str(); } // debug
		Response getResponse() const { return _response; }
};
