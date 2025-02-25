/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/24 15:18:55 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

/**
 * @class Response
 * @brief Represents an HTTP response with status, headers, body, and debug information.
 */
class Response
{
	private:
		string _status;
		string _header;
		string _body;

		map<string, string> _status_line; // status code to reason phrase
		void initStatusLine();
		
		public:
		Response();
		Response(string status);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();
		
		void setStatus(string status);
		void setHeader(string header);
		void setBody(string body);
		void setErrorBody();

		string getResponse();
		string getStatus();
		string getReason();
		string getHeader();
		string getBody();

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
