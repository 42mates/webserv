/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_cgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:33:01 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 17:09:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

char**	Request::initEnv()
{
	char **env;
	map<string, string> cgi_env;

	cgi_env["_QUERY"] = _query;
	cgi_env["_BODY"] = _body;
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
	access_log << "Executing script " << args[1] << endl;
	int pipefd[2];
	if (pipe(pipefd) == -1)
		throw ResponseException(Response("500"), "Pipe creation failed");

	pid_t pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		throw ResponseException(Response("500"), "Fork failed");
	}

	if (pid == 0) // Child process
	{
		close(pipefd[0]); // Close read end of the pipe
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
		close(pipefd[1]);

		if (execve(args[0], args, env) == -1)
		{
			error_log << "execve failed on \"" << args[0] << "\": " << strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(pipefd[1]); // Close write end of the pipe

		string output;
		char buffer[1024];
		ssize_t bytes_read;

		while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			output += buffer;
		}

		close(pipefd[0]);

		int status;
		struct timespec timeout;
		timeout.tv_sec = CGI_TIMEOUT; // Set timeout to 5 seconds
		timeout.tv_nsec = 0;

		int ret;
		while ((ret = waitpid(pid, &status, WNOHANG)) == 0)
		{
			nanosleep(&timeout, NULL);
			if ((ret = waitpid(pid, &status, WNOHANG)) == 0)
			{
				kill(pid, SIGKILL); // Kill the process if it exceeds the timeout
				throw ResponseException(Response("504"), "CGI script execution timed out");
			}
		}

		if (ret == -1)
			throw ResponseException(Response("500"), "Error while waiting for CGI script");

		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			throw ResponseException(Response("502"), "CGI script execution failed: " + output);

		return output;
	}

	return ""; // Should never reach here
}

Response Request::handle_cgi()
{
	Response	response;
	string		script_output;
	char		**args = new char*[3];
	char		**env = initEnv();

	args[0] = new char[17];
	strcpy(args[0], "/usr/bin/python3");
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
		error_log << "handle_cgi(): " << e.what() << endl;
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
