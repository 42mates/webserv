/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_chunked.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:34:19 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/28 18:01:58 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**
 * @brief Exception class for throwing Response objects.
 */
class ChunkedException : public exception
{
	private:
		string _debug;
	public:
		ChunkedException( string debug) : _debug(debug) {}
		~ChunkedException() throw() {}

		const char *what() const throw() { return _debug.c_str(); }
};


static size_t getChunkSize(string str)
{
	size_t end = str.find(CRLF);
	if (end == string::npos || end == 0)
		throw ChunkedException("no chunk size");

	string expression = str.substr(0, end);

	transform(expression.begin(), expression.end(), expression.begin(), ::tolower);
	for (string::iterator it = expression.begin(); it != expression.end(); ++it)
	{
		if (!::isxdigit(*it))
			throw ChunkedException("not hex digit in chunk size");
	}

	size_t chunk_size = strtol(expression.c_str(), NULL, 16);
	if (chunk_size < 0)
		throw ChunkedException("negative chunk size");

	return chunk_size;
}

string decodeOneChunk(string str)
{
	string decoded = "";
	size_t pos = 0;
	size_t chunk_size;

	chunk_size = getChunkSize(str);
	if (chunk_size == 0)
		return decoded;

	pos = str.find(CRLF) + 2;
	if (pos == string::npos)
		throw ChunkedException("no end of chunk size");
	else if (pos + chunk_size > str.size())
		throw ChunkedException("too large chunk size");
	else if (str.substr(pos + chunk_size, 2) != CRLF)
		throw ChunkedException("no CRLF at the end of the chunk");

	decoded = str.substr(pos, chunk_size);
	return decoded;
}

void Request::decodeChunked(string chunk)
{
	size_t pos = 0;
	string decoded;

	while (pos < chunk.size())
	{
		try
		{
			decoded = decodeOneChunk(chunk.substr(pos));
			if (decoded.empty())
			{
				_is_complete_request = true;
				return ;
			}
			pos += chunk.find(CRLF, pos) + 2 + decoded.size() + 2;
			_body_stream << decoded;
			_raw_body = chunk.substr(pos);
		}
		catch(const ChunkedException& e)
		{
			cerr << "Chunked: " <<  e.what() << endl;
		}
	}
}