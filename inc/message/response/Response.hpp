/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 15:30:26 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

#include "AMessage.hpp"

/**
 * @class Response
 * @brief Represents an HTTP response with status, headers, body, and debug information.
 */
class Response : public AMessage
{
	private:
		string _status;
		string _body;
		//map<string, string> _header; //inherited from AMessage

		map<string, string> _status_line; // status code to reason phrase
		void initStatusLine();

		void initHeaderFields();

		public:
		Response();
		Response(string status);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		string	headerToString();
		string	addCRLF(string str);
		
		// SETTERS
		void	setStatus(string status);
		void	setHeaderValue(string key, string value);
		void	setBody(string body);
		void	setErrorBody();
		void	setDate();

		// GETTERS
		string	getResponse();
		string	getStatus();
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
