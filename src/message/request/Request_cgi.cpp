/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_cgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:33:01 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/25 14:31:15 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

char**	Request::initEnv()
{
	char **env;
	map<string, string> cgi_env;

	cgi_env["_QUERY"] = _query;
	cgi_env["_METHOD"] = _method;
	if (_header.find("content-type") != _header.end())
		cgi_env["_CONTENT_TYPE"] = _header["content-type"];
	if (_header.find("content-length") != _header.end())
		cgi_env["_CONTENT_LENGTH"] = _header["content-length"];
	cgi_env["_SCRIPT_NAME"] = _uri;
	cgi_env["_SERVER_NAME"] = WEBSERV_PUBLIC_NAME;
	cgi_env["_SERVER_PORT"] = itostr(_server_conf.port);

	env = new char*[cgi_env.size() + 1];
	size_t i = 0;
	for (map<string, string>::iterator it = cgi_env.begin(); it != cgi_env.end(); ++it, ++i)
	{
		string entry = it->first + "=" + it->second;
		env[i] = new char[entry.size() + 1];
		strcpy(env[i], entry.c_str());
	}
	env[i] = NULL;
	return env;
}

string	executeScript(char **args, char **env)
{
	(void)env;
	cout << "executing " << args[0] << " " << args[1] << endl;
	
	return "Content-Type: text/html\n\n<html><body><h1>CGI SCRIPT</h1></body></html>";
}

Response Request::handle_cgi()
{
	Response	response;
	string		script_output;
	char		**args = new char*[3];
	char		**env = initEnv();

	args[0] = new char[8];
	strcpy(args[0], "python3");
	if (_route_conf.cgi_path.empty())
		throw ResponseException(Response("500"), "CGI script not set in configuration file");
	args[1] = new char[_route_conf.cgi_path.size() + 1];
	strcpy(args[1], _route_conf.cgi_path.c_str());
	args[2] = NULL;

	try
	{
		script_output = "HTTP/1.1 200 OK\n" + executeScript(args, env);
		response.parseResponse(script_output);
	}
	catch(const ResponseException& e)
	{
		cerr << e.what() << endl;
		response = e.getResponse();
	}

	for (size_t i = 0; args[i]; ++i)
		delete[] args[i];
	delete[] args;
	for (size_t i = 0; env[i]; ++i)
		delete[] env[i];
	delete[] env;

	return response;
}
