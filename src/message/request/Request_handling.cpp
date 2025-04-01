/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:10:07 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:29:10 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::checkStartLine()
{
	if (_method.empty() || _uri.empty() || _version.empty())
		throw ResponseException(Response("400"), "request start line has empty field (parsing functions were not called)");

	// get query string
	if (_uri.find("?") != string::npos)
	{
		_query = _uri.substr(_uri.find("?") + 1);
		_uri = _uri.substr(0, _uri.find("?"));
	}
}

void Request::checkMethod()
{
	if (std::find(_route_conf.methods.begin(), _route_conf.methods.end(), _method) == _route_conf.methods.end())
	{
		Response response("405");
		response.setHeaderValue("allow", Config::getAllowedMethods(_route_conf.methods));
		throw ResponseException(response, "unknown method");
	}
}

void Request::checkHeader()
{
	if (_header["host"].empty())
		throw ResponseException(Response("400"), "missing required header field \"Host\"");
	if (!_header["expect"].empty() && _header["expect"] != "100-continue")
		throw ResponseException(Response("417"), "expect value not supported");
	//if (_header["content-length"].empty())
	//	throw ResponseException(Response("400"), "missing required header field \"Content-Length\"");

	if (!_route_conf.http_redirect.first.empty())
	{
		Response r(_route_conf.http_redirect.first);
		r.setHeaderValue("location", _route_conf.http_redirect.second);
		throw ResponseException(r, "redirecting to " + _route_conf.http_redirect.second);
	}
	if (!_route_conf.alias.empty())
	{
		if (_uri.find(_route_conf.path) == 0)
			_uri = _uri.substr(_route_conf.path.length());
		_path = getFilePath(_route_conf.alias + _uri);
	}
	else
		_path = getFilePath(_route_conf.root + _uri);
}

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
			else if (access(paths[i].c_str(), R_OK) != 0 && errno == EACCES)
				throw ResponseException(Response("403"), "getFilePath(): access denied to file " + paths[i]);
		}
	}
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode))
		throw ResponseException(Response("404"), "getFilePath(): path \"" + path + "\" is a directory");
	throw ResponseException(Response("404"), "getFilePath(): could not find file \"" + path + "\"");	
}

Response Request::handleRequest(ServerConfig &server_conf)
{
	Response response;

	//if (_method != "GET") // debug (avoiding GET printing)
		//this->print(); //!leave this line for debugging purposes during correction
	
	try
	{
		if (!_is_complete_request)
			throw ResponseException(Response("400"), "incomplete request");
		checkStartLine();

		_server_conf = server_conf;
		_route_conf = getBestRoute(_server_conf, _uri);
		
		checkMethod();
		checkHeader();



		if (_uri.size() >= 3 && _uri.substr(_uri.size() - 3) == ".py") // if file is cgi
			response =  handle_cgi();
		else
			response = (this->*_method_handling[_method])(); // call the method handling function
	}
	catch(const ResponseException& e)
	{
		cerr <<  "handleRequest(): " << e.what() << endl;
		response = e.getResponse();
	}
	catch(const exception& e)
	{
		cerr << "handleRequest() exception: " << e.what() << endl;
		response = Response("500");
	}

	if(_server_conf.error_pages.find(response.getCode()) != _server_conf.error_pages.end())
		response.setErrorBody(_server_conf, _route_conf.root);

	return response;
}
