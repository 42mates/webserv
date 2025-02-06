/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_decode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:34:19 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/06 16:28:35 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static size_t getChunkSize(string body)
{
	size_t end = body.find("\r\n");
	if (end == string::npos || end == 0)
		throw runtime_error("debug: 400 Bad Request (no chunk size)");

	string expression = body.substr(0, end);

	transform(expression.begin(), expression.end(), expression.begin(), ::tolower);
	if (!all_of(expression.begin(), expression.end(), ::isxdigit))
		throw runtime_error("debug: 400 Bad Request (not hex digit in chunk size)");
	
	size_t chunk_size = strtol(expression.c_str(), NULL, 16);
	if (chunk_size < 0)
		throw runtime_error("debug: 400 Bad Request (negative chunk size)");

	return chunk_size;
}

string Request::decodeChunked(string body)
{
	string decoded;
	size_t pos = 0;
	size_t chunk_size;
	string chunk;

	while (pos < body.size())
	{
		chunk_size = getChunkSize(body.substr(pos));
		if (chunk_size == 0)
			break;

		pos = body.find("\r\n", pos) + 2;
		if (pos == string::npos)
			throw runtime_error("debug: 400 Bad Request (no end of chunk size)");
		else if (chunk_size + pos > body.size())
			throw runtime_error("debug: 400 Bad Request (too large chunk size)"); 

		chunk = body.substr(pos, chunk_size);
		decoded += chunk;
		pos += chunk_size + 2;
	}
	return decoded;
}
