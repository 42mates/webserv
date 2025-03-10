/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_status.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:30:01 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/10 15:19:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

bool Request::isCompleteHeader(string raw_request)
{
	size_t end = 0;
	string line;

	while ((end = raw_request.find("\r\n")) == 0)
		raw_request.erase(0, end + 2);
	if (raw_request.empty())
		return false;

	if (raw_request.find("\r\n\r\n") == string::npos)
		return false;
	return true;
}

/**
 * @brief Checks if the HTTP request is complete based on the method, headers, and body. 
 * If so, sets the `_is_complete_request` flag to true.
 */

void Request::setIsCompleteRequest()
{
	if (!_header_parsed || _is_complete_request)
		return ;

	if (_method == "GET" || _method == "HEAD")
		_is_complete_request = true;
	else if (_header["transfer-encoding"] == "chunked")
	{
		string body = _raw_request.substr(_raw_request.find("\r\n\r\n") + 4);
		if (body.find("\r\n0\r\n\r\n") != string::npos)
		_is_complete_request = true;
	}
	else if (!_header["content-length"].empty())
	{
		string body = _raw_request.substr(_raw_request.find("\r\n\r\n") + 4);
		cerr << BBLUE << "content-length: " << BLUE << _header["content-length"] << endl;
		cerr << BBLUE << "body size:      " << BLUE << body.size() << NC << endl;
		
		if (body.size() >= strtoul(_header["content-length"].c_str(), NULL, 10))
			_is_complete_request = true;
	}

	//! in case of a POST request, the content-length header is mandatory
	//? where should we place the catch ?
	if (_header["content-length"].empty())
		throw ResponseException(Response("411"), "missing required header field \"Content-Length\"");
}
 