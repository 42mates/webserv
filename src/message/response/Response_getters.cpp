/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_getters.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:29:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 15:12:42 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

string Response::getResponse()
{
	if (!_body.empty())
		_header["content-length"] = itostr(_body.size());

	return addCRLF("HTTP/1.1 " + _code + " " + _status_line[_code] + "\n" + headerToString() + "\n") + _body;
}

string Response::getCode()
{
	return _code;
}

string Response::getReason()
{
	return _status_line[_code];
}

string Response::getHeader()
{
	return headerToString();
}

string Response::getHeader(string key)
{
	return _header[key];
}

string Response::getBody()
{
	return _body;
}
