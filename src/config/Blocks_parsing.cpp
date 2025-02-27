/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/11 14:17:26 by sokaraku         ###   ########.fr       */
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

void ServerBlock::parseRoot(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseRoot(val);
}

void ServerBlock::parseIndexFile(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseIndexFile(val);
}

void ServerBlock::parseReturn(vector<ConfigToken> val)
{
	LocationBlock block(_config->routes["/"], _filepath);
	block.parseReturn(val);
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
	_config->server_names.resize(0); //! just for testing purposes
	for (size_t i = 0; i < val.size(); i++)
		_config->server_names.push_back(val[i].token);
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
			long tmp = atol(val[i].token.c_str());
			if (tmp < 300 || tmp > 599)
				throw runtime_error(string("value ") + qString(val[i].token) + string(" must be between 300 and 599 in ") + _filepath + ":" + itostr(val[i].line));
			_config->error_pages[tmp] = val.back().token;
		}
	}
}

void ServerBlock::parseClientMaxBodySize(vector<ConfigToken> val)
{
	int		bytes_multiplier = 1024;
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
		char	tmp = toupper(val[0].token.at(first_not_of));
		if (tmp != 'K' && tmp != 'M' && tmp != 'G')
			throw runtime_error(err_msg);
		(tmp == 'M') ? bytes_multiplier *= 1024 : bytes_multiplier *= (1024 * 1024);
		if (val[0].token.size() != first_not_of + 1) //* means that there is something after the unit
			throw runtime_error(err_msg);
	}
	istringstream	iss(val[0].token);
	iss >> bytes;
	if (iss.fail())
		throw runtime_error("string stream conversion error in parseClientMaxBodySize()");
	bytes *= bytes_multiplier;
	if (bytes == 0)
		_config->client_max_body_size = string::npos; //* disables the limit
	else
		_config->client_max_body_size = bytes;
	//? set a maximum value (so that it doesn't exceed system limits)
}

ServerBlock::~ServerBlock(void) {}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(vector<ConfigToken> val)
{
	//todo only one argument ✅
	//todo starts with / (but nginx doesnt throw an error when it is not)
	//? so does that cause error at the execution?

	if (val.size() != 1 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"root\" directive in ") + _filepath + ":" + itostr(val[0].line));
	_config->root = val[0].token;
}

void LocationBlock::parseMethods(vector<ConfigToken> val)
{
	if (val.size() == 0 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"methods\" directive ") + _filepath + ":" + itostr(val[0].line));
	_config->methods.resize(0); //! just for testing purposes
	for (size_t i = 0; i < val.size(); i++)
	{
		string	tmp(val[i].token);
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp != "GET" && tmp != "POST" && tmp != "DELETE")
			throw runtime_error(METHOD_UNKNOWN + qString(val[i].token) + " in \"methods\" directive " + _filepath + ":" + itostr(val[i].line));
		_config->methods.push_back(tmp);
	}
}

void LocationBlock::parseDirectoryListing(vector<ConfigToken> val)
{
	//! is triggered only if no index_file is found
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
	_config->index_file.resize(0); //! just for testing purposes
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

//TODO when there is no return code, 302 is assumed
//? Store error code somewhere?
void LocationBlock::parseHttpRedirect(vector<ConfigToken> val)
{
	long	return_value;
	string	return_string = "\"return\" directive in ";

	if (val.size() == 0 || val.size() > 2 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + return_string + _filepath + ":" + itostr(val[0].line)); //?nginx doesn't throw an error when one arg only

	if (val.size() == 1)
	{
		_config->http_redirect = val[0].token;
		return ;
	}

	if (val[0].token.find_first_not_of("0123465789") != string::npos)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in " + return_string + _filepath + ":" + itostr(val[0].line));

	return_value = strtol(val[0].token.c_str(), NULL, 10);
	if (return_value < 300 || return_value > 399)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in " + return_string + _filepath + ":" + itostr(val[0].line));
	_config->http_redirect = val[1].token;

}

void LocationBlock::parseReturn(vector<ConfigToken> val)
{
	long	return_value;

	if (val.size() == 0 || val.size() > 2 || val[0].token.empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"return\" directive ") + _filepath + ":" + itostr(val[0].line));

	if (val[0].token.find_first_not_of("0123465789") != string::npos)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in " + _filepath + ":" + itostr(val[0].line));

	return_value = strtol(val[0].token.c_str(), NULL, 10);
	if (return_value > 999)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0].token) + " in " + _filepath + ":" + itostr(val[0].line));
}


LocationBlock::~LocationBlock(void) {}
