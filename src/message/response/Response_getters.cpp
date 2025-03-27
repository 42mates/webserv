/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_getters.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:29:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 11:51:58 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

string Response::getResponse()
{
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
