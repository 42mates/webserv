/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 16:45:44 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"


/*********** UTILS FOR TESTING *************** */

void printMap(const map<int, string>& m)
{
    for (map<int, string>::const_iterator it = m.begin(); it != m.end(); ++it)
        cout << "map[" << it->first << "] : " << it->second << '\n';
}

void printVector(const vector<ConfigToken>& v)
{
	for (size_t i = 0; i < v.size(); i++)
		cout << v.at(i).token << '\n';
}


/*********** SERVER BLOCK ***********/

/**
 * @brief Puts a string between double quotes and returns it.
 */
string	qString(string to_quote) { return "\"" + to_quote + "\""; }

void ServerBlock::parseAlias(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseAlias(val);
	_config->alias = val[0].token;
}

void ServerBlock::parseRoot(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseRoot(val);
	_config->root = val[0].token;
}

void ServerBlock::parseIndexFile(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseIndexFile(val);
	for (size_t i = 0; i < val.size(); i++)
		_config->index_file.push_back(val[i].token);
}

void ServerBlock::parseReturn(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseReturn(val);
	_config->http_redirect = block.getConfig()->http_redirect;
}


static void	findIpAndPort(string &ip, string &port, const string &token)
{
	bool	no_colon = (token.find(':') == string::npos) ? true : false;

	if (no_colon)
	{
		if (countOccurrences(token, '.') != 0)
			ip = token, port = DEFAULT_PORT;
		else
			port = token, ip = DEFAULT_HOST;
		return ;
	}
	ip = token.substr(0, token.find(':'));
	port = token.substr(token.find(':') + 1);
}

static bool checkIpFormat(const string &ip)
{
    istringstream iss(ip);
    string token;
    vector<size_t> components;

    while (getline(iss, token, '.'))
	{
        istringstream components_stream(token);
        size_t component;
        components_stream >> component;
        if (components_stream.fail() || component > 255)
            return false;
        components.push_back(component);
    }
    return (components.size() == 4);
}

void ServerBlock::parseListen(vector<ConfigToken> val)
{
	if (val.size() == 0 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"listen\" directive in ") + _filepath + ":" + itostr(val[0].line));
	if (val.size() > 1)
		throw runtime_error(INVALID_PARAMETER + qString(val[1].token) + " of the \"listen\" directive in " + _filepath + ":" + itostr(val[1].line));

	if (val[0].token.find_first_not_of("0123456789.:") != string::npos)
		throw runtime_error(HOST_NOT_FOUND + qString(val[0].token) + string(" of the \"listen\" directive in ") + _filepath + ":" + itostr(val[0].line));

	string	ip, port;
	findIpAndPort(ip, port, val[0].token);

	if (ip.find_first_not_of("0123456789.") != string::npos || port.find_first_not_of("0123456789") != string::npos)
		throw runtime_error(HOST_NOT_FOUND + qString(val[0].token) + string(" of the \"listen\" directive in ") + _filepath + ":" + itostr(val[0].line));
	
	if (checkIpFormat(ip) == false)
		throw runtime_error(INVALID_IP + qString(ip) + string(" of the \"listen\" directive in ") + _filepath + ":" + itostr(val[0].line));

	long tmp_port = strtol(port.c_str(), NULL, 10);
	if (tmp_port > 65535)
		throw runtime_error(INVALID_PORT + qString(val[0].token) + string(" of the \"listen\" directive in ") + _filepath + ":" + itostr(val[0].line));
	_config->port = tmp_port;
	_config->host = ip;
}

void ServerBlock::parseServerName(vector<ConfigToken> val)
{
	if (val.size() == 0 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"server_name\" directive in ") + _filepath + ":" + itostr(val[0].line));
	_config->server_names.resize(0);
	for (size_t i = 0; i < val.size(); i++)
		_config->server_names.push_back(val[i].token);
}

static bool isValidHTTPError(long code)
{
	return code ==	(code >= 300 && code <= 307) ||
					(code >= 400 && code <= 417) ||
					(code >= 500 && code <= 505);
}

void ServerBlock::parseErrorPage(vector<ConfigToken> val)
{
	if (val.size() < 2)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"error_page\" directive in ") + _filepath + ":" + itostr(val[0].line));

	for (size_t i = 0; i < val.size(); i++)
	{
		if (i == val.size() - 1)
			continue ;
		else
		{
			if (val[i].token.find_first_not_of("0123456789") != string::npos)
				throw runtime_error(INVALID_VALUE + qString(val[i].token)	 + string(" in ") + _filepath + ":" + itostr(val[i].line));
			if (!isValidHTTPError(atol(val[i].token.c_str())))
				throw runtime_error(string("value ") + qString(val[i].token) + string(" must be between 300 and 599 in ") + _filepath + ":" + itostr(val[i].line));
			_config->error_pages[val[i].token] = val.back().token;
		}
	}
}

void ServerBlock::parseClientMaxBodySize(vector<ConfigToken> val)
{
	size_t	bytes_multiplier = 1;
	string	err_msg = qString("client_max_body_size") + " directive invalid value in " + _filepath + ":" + itostr(val[0].line);
	size_t	first_not_of;
	size_t	bytes;

	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"client_max_body_size\" directive in ") + _filepath + ":" + itostr(val[0].line));

	if (isdigit(val[0].token.at(0)) == false)
		throw runtime_error(err_msg);

	first_not_of = val[0].token.find_first_not_of("0123456789");
	if (first_not_of != string::npos)
	{
		char	unit = toupper(val[0].token.at(first_not_of));
		if (unit != 'K' && unit != 'M' && unit != 'G')
			throw runtime_error(err_msg);

		if (unit == 'K') bytes_multiplier = 1000;
		if (unit == 'M') bytes_multiplier = 1000 * 1000;
		if (unit == 'G') bytes_multiplier = 1000 * 1000 * 1000;
		
		if (val[0].token.size() != first_not_of + 1)
			throw runtime_error(err_msg);
	}
	istringstream	iss(val[0].token);
	iss >> bytes;
	if (iss.fail())
		throw runtime_error("string stream conversion error in parseClientMaxBodySize()");
	bytes *= bytes_multiplier;
	if (bytes >= 0)
		_config->client_max_body_size = bytes;
}

ServerBlock::~ServerBlock(void) {}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(vector<ConfigToken> val)
{

	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"root\" directive in ") + _filepath + ":" + itostr(val[0].line));
	_config->root = val[0].token;
}

void LocationBlock::parseAlias(vector<ConfigToken> val)
{

	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"alias\" directive in ") + _filepath + ":" + itostr(val[0].line));
	_config->alias = val[0].token;
}

void LocationBlock::parseMethods(vector<ConfigToken> val)
{
	if (val.size() == 0 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"methods\" directive ") + _filepath + ":" + itostr(val[0].line));
	_config->methods.clear();
	for (size_t i = 0; i < val.size(); i++)
	{
		string	tmp(val[i].token);
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (   tmp != "GET"
			&& tmp != "POST"
			&& tmp != "DELETE"
			&& tmp != "HEAD")
			throw runtime_error(METHOD_UNKNOWN + qString(val[i].token) + " in \"methods\" directive " + _filepath + ":" + itostr(val[i].line));
		_config->methods.push_back(tmp);
	}
}

void LocationBlock::parseDirectoryListing(vector<ConfigToken> val)
{
	string	auto_index("\"auto_index\" directive ");

	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + auto_index + _filepath + ":" + itostr(val[0].line));

	string	tmp = val[0].token;
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	if (tmp != "off" && tmp != "on")
		throw runtime_error(INVALID_VALUE + qString(val[0].token) + " in " + auto_index + _filepath + ":" + itostr(val[0].line));
}

void LocationBlock::parseIndexFile(vector<ConfigToken> val)
{
	if (val.size() < 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"index_file\" directive ") + _filepath + ":" + itostr(val[0].line));
	_config->index_file.resize(0);
	for (size_t i = 0; i < val.size(); i++)
		_config->index_file.push_back(val[i].token);
}

void LocationBlock::parseCgiPath(vector<ConfigToken> val)
{
	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"cgi_path\" directive ") + _filepath + ":" + itostr(val[0].line));
	_config->cgi_path = val[0].token;
}

void LocationBlock::parseUploadDir(vector<ConfigToken> val)
{
	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"upload_dir\" directive ") + _filepath + ":" + itostr(val[0].line));
	_config->upload_dir = val[0].token;

}

void LocationBlock::parseReturn(vector<ConfigToken> val)
{
	if (val.size() == 0 || val.size() > 2 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"return\" directive in ") + _filepath + ":" + itostr(val[0].line));

	if (val[0].token.find_first_not_of("0123465789") != string::npos)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in \"return\" directive in " + _filepath + ":" + itostr(val[0].line));

	short num = strtol(val[0].token.c_str(), NULL, 10);
	if (num < 300 || num > 307)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in \"return\" directive in " + _filepath + ":" + itostr(val[0].line));
	_config->http_redirect.first = val[0].token;
	_config->http_redirect.second = (val.size() == 2) ? val[1].token : "";
}

LocationBlock::~LocationBlock(void) {}
