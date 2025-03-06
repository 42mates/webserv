/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/06 16:19:04 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**
 * @brief Reads the entire content of a file into a string.
 */
string Request::getFileContent()
{
	ifstream file(_path.c_str());
	if (!file.is_open() || !file.good())
		throw ResponseException(Response("404"), "getFileContent(): could not open file " + _path);
	
	string content;
	try
	{
		content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	}
	catch(const std::exception& e)
	{
		throw ResponseException(Response("404"), "getFileContent(): " + string(e.what()));
	}

	file.close();

	return content;
}

Response Request::handleGet()
{
	Response response;
	try
	{
		string file = getFileContent();
		response.setBody(file);
		response.setStatus("200");
		//response.setHeader(???);
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << e.what() << endl;
		response = e.getResponse();
		cout << RED << "REQUEST DENIED 😱" << NC << endl;
		return response;
	}
	cout << GREEN << "REQUEST ACCEPTED 😎" << NC << endl;
	return response;
}

Response Request::handleHead()
{
	Response response;

	try
	{
		string file = getFileContent();
		response.setBody("");
		response.setStatus("200");
		//response.setHeader(???);
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleGet(): " << e.what() << endl;
		response = e.getResponse();
		cout << RED << "REQUEST DENIED 😱" << NC << endl;
		return response;
	}
	cout << GREEN << "REQUEST ACCEPTED 😎" << NC << endl;
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