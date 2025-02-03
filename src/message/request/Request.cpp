/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/31 17:39:48 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

RequestHandler::RequestHandler()
{
	_header["Accept"];
	_header["Accept-Charset"];
	_header["Accept-Encoding"];
	_header["Accept-Language"];
	_header["Authorization"];
	_header["Expect"];
	_header["From"];
	_header["Host"];
	_header["If-Match"];
	_header["If-Modified-Since"];
	_header["If-None-Match"];
	_header["If-Range"];
	_header["If-Unmodified-Since"];
	_header["Max-Forwards"];
	_header["Proxy-Authorization"];
	_header["Range"];
	_header["Referer"];
	_header["TE"];
	_header["User-Agent"];
}
