/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_getters.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:29:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/06 15:56:02 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

string Response::getResponse()
{
	return addCRLF("HTTP/1.1 " + _status + "\n" + headerToString() + "\n") + _body;
}

string Response::getStatus()
{
	return _status;
}

string Response::getReason()
{
	return _status_line[_status];
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
