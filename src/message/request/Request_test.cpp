/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:31:20 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/10 15:04:50 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**
 * @brief Debug function that tests whether or not the request can be successfully parsed.
 */
void Request::testParsing()
{
	try
	{
		//setIsCompleteRequest(true);
		parseRequest(getTestRequest());
		cout << "Method:  " << _method << endl;
		cout << "URI:     " << _uri << endl;
		cout << "Version: " << _version << endl;
		cout << "Header:  " << endl;
		for (map<string, string>::iterator it = _header.begin(); it != _header.end(); it++)
		{
			if (!it->second.empty())	
				cout << "\t" << it->first << ": " << it->second << endl;
		}
		cout << "Body: " << _body << endl;
	}
	catch (ResponseException &e)
	{
		Response r = e.getResponse();
		cout << r.getStatus() << " " << r.getReason() << endl;
		//cout << r.getBody() << endl;
		cout << e.what() << endl;
	}
}
