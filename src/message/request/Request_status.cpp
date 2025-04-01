/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_status.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:30:01 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:55:35 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

bool Request::isCompleteHeader(string raw_request)
{
	size_t end = 0;
	string line;

	while ((end = raw_request.find(CRLF)) == 0)
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

	if (_header_parsed && (_method == "GET" || _method == "HEAD" || _method == "DELETE"))
	{
		_is_complete_request = true;
		return ;
	}
	else if (!_header["content-length"].empty())
	{
		if (getBodySize() > strtoul(_header["content-length"].c_str(), NULL, 10))
			throw ResponseException(Response("400"), "invalid content-length (expected: " + _header["content-length"] + ", got: " + itostr(getBodySize()));
		else if (getBodySize() == strtoul(_header["content-length"].c_str(), NULL, 10))
			_is_complete_request = true;
	}

	if (_header["content-length"].empty() && _header["transfer-encoding"] != "chunked")
		throw ResponseException(Response("411"), "missing required header field \"Content-Length\"");
}
 