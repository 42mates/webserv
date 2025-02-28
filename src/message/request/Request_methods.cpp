/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 17:11:34 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

string Request::getFilePath(const string &path)
{
	vector<string> paths;

	paths.push_back(path);
	for (size_t i = 0; i < _route_conf.index_file.size(); ++i)
		paths.push_back(path + "/" + _route_conf.index_file[i]);
	
	for (size_t i = 0; i < paths.size(); ++i)
	{
		struct stat buffer;
		if (stat(paths[i].c_str(), &buffer) == 0)
		{
			if (S_ISREG(buffer.st_mode) && access(paths[i].c_str(), R_OK) == 0)
				return paths[i];
		}
	}
	throw ResponseException(Response("404"), "getFilePath(): could not find file " + path);	
}

/**
 * @brief Reads the entire content of a file into a string.
 */
string Request::getFile(const string &path)
{
	ifstream file(getFilePath(path).c_str());
	if (!file.is_open() || !file.good())
		throw ResponseException(Response("404"), "getFile(): could not open file " + path);
	
	string content;
	try
	{
		content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	}
	catch(const std::exception& e)
	{
		throw ResponseException(Response("404"), "getFile(): " + string(e.what()));
	}

	file.close();

	return content;
}

Response Request::handleGet()
{
	Response response;
	try
	{
		string file = getFile(_route_conf.root);
		response.setBody(file);
		response.setStatus("200");
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << e.what() << endl;
		response = e.getResponse();
	}
	cout << GREEN << "RESPONSE ACCEPTED 😎" << NC << endl;
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