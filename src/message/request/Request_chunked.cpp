/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_chunked.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:34:19 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/10 11:39:35 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

size_t getChunkSize(string body)
{
	size_t end = body.find("\r\n");
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

	while (pos < body.size())
	{
		chunk_size = getChunkSize(body.substr(pos));
		if (chunk_size == 0)
			break;

		pos = body.find("\r\n", pos) + 2;
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
