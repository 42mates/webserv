/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_print.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:59:59 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/28 17:07:11 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::printStartLine()
{
	cout << _method << " " <<_uri << " " <<_version << YELLOW << "\\r\\n" << NC << endl;
}

void Request::printHeader()
{
	for (map<string, string>::iterator it = _header.begin(); it != _header.end(); ++it)
	{
		if (!it->second.empty())
			cout << it->first << ": " << it->second << YELLOW << "\\r\\n" << NC << endl;
	}
}

void Request::printBody()
{
	if (_body.empty())
	{
		cout << BYELLOW << "[WEBSERV: EMPTY BODY]" << NC << endl;
		return;
	}

	for (size_t i = 0; i < _body.size(); ++i)
	{
		if (_body[i] == '\r')
			cout << YELLOW << "\\r" << NC;
		else if (_body[i] == '\n')
			cout << YELLOW << "\\n\n" << NC;
		else
			cout << _body[i];
	}
	if (!_body.empty() && _body[_body.size() - 1] != '\n')
		cout << BYELLOW << "%" << endl;
}

void Request::printBody(string &body)
{
	if (body.empty())
	{
		cout << BBLUE << "[WEBSERV: EMPTY BODY]" << NC << endl;
		return;
	}

	for (size_t i = 0; i < body.size(); ++i)
	{
		if (body[i] == '\r')
			cout << BLUE << "\\r" << NC;
		else if (body[i] == '\n')
			cout << BLUE << "\\n\n" << NC;
		else
			cout << body[i];
	}
	if (!body.empty() && body[body.size() - 1] != '\n')
		cout << BBLUE << "%" << endl;
}

void Request::print()
{
	cout << YELLOW << "------PARSED REQUEST------" << NC << endl;
	printStartLine();
	printHeader();
	cout << BYELLOW << "\\r\\n" << NC << endl;
	printBody();
	cout << YELLOW << "--------------------------" << NC << endl;
}

void Request::printRaw()
{
	cout << RED << "------RAW REQUEST------" << NC << endl;

	size_t i = 0;
	for (; i < _raw_request.size(); ++i) 
	{
		if (_raw_request[i] == '\r')
			cout << RED << "\\r" << NC;
		else if (_raw_request[i] == '\n')
			cout << RED << "\\n" << NC << endl;
		else
			cout << _raw_request[i];
	}
	if (--i == _raw_request.size() - 1 && _raw_request[i] != '\n')
		cout << BRED << "%\n" << NC;
	
	cout << RED << "-----------------------" << NC << endl;
}