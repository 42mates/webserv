/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/13 18:03:22 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**
 * @brief Reads the entire content of a file into a string.
 */
static string getFile(const string &path)
{
	ifstream file(path.c_str());
	if (!file.is_open() || !file.good())
		throw ResponseException(Response("404"), "getFile(): could not open file " + path);
	
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	file.close();

	return content;
}

Response Request::handleGet()
{
	Response response;

	try
	{
		string file = getFile(_uri);
		response.setBody(file);
		response.setStatus("200");
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleGet(): " << e.what() << endl;
		response = e.getResponse();
	}

	return response;
}

Response Request::handleHead()
{
	Response response;

	try
	{
		string file = getFile(_uri);
		response.setBody("");
		//response.setHeader(???);
		response.setStatus("200");
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleGet(): " << e.what() << endl;
		response = e.getResponse();
	}

	return response;
}

Response Request::handlePost()
{
	Response response;

	return response;
}

Response Request::handleDelete()
{
	Response response;

	return response;
}