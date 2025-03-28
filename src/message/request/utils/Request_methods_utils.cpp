/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_methods_utils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:10:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/17 13:37:48 by mbecker          ###   ########.fr       */
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
	catch(const exception& e)
	{
		throw ResponseException(Response("404"), "getFileContent(): " + string(e.what()));
	}

	file.close();

	return content;
}
