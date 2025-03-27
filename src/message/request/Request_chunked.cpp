/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_chunked.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:34:19 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/17 15:10:20 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static size_t getChunkSize(string body)
{
	size_t end = body.find(CRLF);
	if (end == string::npos || end == 0)
		throw ResponseException(Response("400"), "no chunk size");

	string expression = body.substr(0, end);

	transform(expression.begin(), expression.end(), expression.begin(), ::tolower);
	for (string::iterator it = expression.begin(); it != expression.end(); ++it)
	{
		if (!::isxdigit(*it))
			throw ResponseException(Response("400"), "not hex digit in chunk size");
	}
	
	size_t chunk_size = strtol(expression.c_str(), NULL, 16);
	if (chunk_size < 0)
		throw ResponseException(Response("400"), "negative chunk size");

	return chunk_size;
}

string Request::decodeChunked(string body)
{
	string decoded;
	size_t pos = 0;
	size_t chunk_size;
	string chunk;

	// remove any data after the last chunk
	body.erase(body.find("0\r\n\r\n") + 5);

	while (pos < body.size())
	{
		chunk_size = getChunkSize(body.substr(pos));
		if (chunk_size == 0)
			break;

		pos = body.find(CRLF, pos) + 2;
		if (pos == string::npos)
			throw ResponseException(Response("400"), "no end of chunk size");
		else if (chunk_size + pos > body.size())
			throw ResponseException(Response("400"), "too large chunk size");

		chunk = body.substr(pos, chunk_size);
		decoded += chunk;
		pos += chunk_size + 2;
	}
	return decoded;
}
