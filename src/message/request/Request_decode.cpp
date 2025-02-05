/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_decode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:34:19 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/05 15:57:42 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

size_t getChunkSize(string body, size_t pos)
{
	size_t chunk_size = strtol(body.c_str() + pos, NULL, 16);
	if (chunk_size == 0)
		throw runtime_error("debug: 400 Bad Request (invalid chunk size)");
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
		chunk_size = getChunkSize(body, pos);
		if (chunk_size == 0)
			break;
		pos = body.find("\r\n", pos) + 2;
		chunk = body.substr(pos, chunk_size);
		decoded += chunk;
		pos += chunk_size + 2;
	}
	return decoded;
}
