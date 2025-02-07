/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/07 13:11:06 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

class Response
{
	private:
		string _status;
		string _header;
		string _body;
		string _debug;

		map<string, string> _status_line; // key is status code, value is reason-phrase
		void initStatusLine();
		void setErrorBody();

	public:
		Response(string status);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		void setHeader(string header);
		void setBody(string body);
		void setDebug(string debug);

		string getResponse();
		string getStatus();
		string getReason();
		string getHeader();
		string getBody();
		string getDebug();

		void print();
};


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
