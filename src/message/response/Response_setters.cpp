/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_setters.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:29:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/17 13:35:06 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::setStatus(string status)
{
	if (_status_line.find(status) == _status_line.end())
		throw runtime_error(string("debug: Response setStatus() used with invalid arg \"") + status + "\"");
	_status = status;
}

void Response::setHeaderValue(string key, string value)
{
	transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (_header.find(key) != _header.end())
		_header[key] = value;
	else
		throw runtime_error(string("debug: Response setHeaderValue() used with invalid key \"") + key + "\"");
}

void Response::setBody(string body)
{
	_body = body;
}

void Response::setErrorBody()
{
	//DEFAULT ERROR BODY WITHOUT CARRIAGE RETURN
	_body = string("<html>\n")
		+ "<head><title>" + _status + " " + _status_line[_status] + "</title></head>\n"
		+ "<body>\n"
		+ "<center><h1>" + _status + " " + _status_line[_status] + "</h1></center>\n"
		+ "<hr><center>webserv/1.0</center>\n"
		+ "</body>\n"
		+ "</html>\n";
}

void Response::setErrorBody(ServerConfig &server_conf, string &root)
{
	map<string, string>::iterator it;
	for (it = server_conf.error_pages.begin(); it != server_conf.error_pages.end(); ++it)
	{
		if (it->first == _status)
		{
			ifstream file((root + it->second).c_str());
			if (file.is_open() && file.good())
			{
				_body.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
				file.close();
				return;
			}
		}
	}
	if (!server_conf.error_pages.empty())
		cerr << "debug: Response setErrorBody() could not set custom error body for " << _status << endl;
}

void Response::setDate()
{
	_header["date"] = getDate();
}
