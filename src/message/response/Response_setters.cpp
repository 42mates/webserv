/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_setters.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:29:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 15:31:26 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::setStatus(string status)
{
	if (_status_line.find(status) == _status_line.end())
		throw invalid_argument(string("debug: Response setStatus() used with invalid arg \"") + status + "\"");
	_status = status;
}

void Response::setHeaderValue(string key, string value)
{
	if (_header.find(key) != _header.end())
		_header[key] = value;
	else
		throw invalid_argument(string("debug: Response setHeaderValue() used with invalid key \"") + key + "\"");
}

void Response::setBody(string body)
{
	_body = body;
}

void Response::setErrorBody()
{
	//DEFAULT ERROR BODY WITHOUT CARRIAGE RETURN
	_body = string("<html>\n")
		+ "<head><title>" + _status + " " + _status_line[_status] + "</title></head>\n"
		+ "<body>\n"
		+ "<center><h1>" + _status + " " + _status_line[_status] + "</h1></center>\n"
		+ "<hr><center>webserv/1.0</center>\n"
		+ "</body>\n"
		+ "</html>\n";
}

void Response::setDate()
{
	_header["date"] = getDate();
}
