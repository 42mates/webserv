/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/21 17:52:37 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"


/*********** UTILS FOR TESTING *************** */

void printMap(const map<int, string>& m)
{
    for (map<int, string>::const_iterator it = m.begin(); it != m.end(); ++it)
        cout << "map[" << it->first << "] : " << it->second << '\n';
}

void printVector(const vector<string>& v)
{
	for (size_t i = 0; i < v.size(); i++)
		cout << v.at(i) << '\n';
}
/*********** SERVER BLOCK ***********/

/**
 * @brief Puts a string between double quotes and returns it.
 */
string	qString(string to_quote) { return "\"" + to_quote + "\""; }

void ServerBlock::parseListen(vector<string> val)
{
	
	if (val.size() == 0 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"listen\" directive in ") + _filepath);
	if (val.size() > 1)
		throw runtime_error(INVALID_PARAMETER + qString(val[1]) + " of the \"listen\" directive in " + _filepath);

	if (val[0].find_first_not_of("0123456789") != string::npos)
		throw runtime_error(HOST_NOT_FOUND + qString(val[0]) + string(" of the \"listen\" directive in ") + _filepath);

	long tmp_port = strtol(val[0].c_str(), NULL, 10);
	if (tmp_port > 65535)
		throw runtime_error(PORT_OUT_OF_BOUND + qString(val[0]) + string(" of the \"listen\" directive in ") + _filepath);
	this->_config->port = tmp_port;
	cout << "parseListen successful ✅\n this->port = "
	<< this->_config->port << "\n";
}

void ServerBlock::parseServerName(vector<string> val)
{
	if (val.size() == 0 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"server_name\" directive in ") + _filepath);
	//todo not empty ✅
	this->_config->server_names.resize(0); //! just for testing purposes
	for (size_t i = 0; i < val.size(); i++)
		this->_config->server_names.push_back(val[i]);
	cout << "parseServerName successful ✅\nserver_names are ";
	printVector(this->_config->server_names);
}

void ServerBlock::parseErrorPage(vector<string> val)
{
	string out_of_bound = " must be between 300 and 599 in ";

	if (val.size() < 2)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"error_page\" directive in ") + _filepath);

	for (size_t i = 0; i < val.size(); i++) //? can val store multiple empty strings at the end of parsing?
	{
		if (i == val.size() - 1)
			continue ;
		else
		{
			if (val[i].find_first_not_of("0123456789") != string::npos)
				throw runtime_error(INVALID_VALUE + qString(val[i])	 + string(" in ") + _filepath);
			long tmp = atol(val[i].c_str());
			if (tmp < 300 || tmp > 599)
				throw runtime_error(string("value ") + qString(val[i]) + out_of_bound + _filepath);
			this->_config->error_pages[tmp] = val.back();
		}
	}
	//todo at least 2 arguments ✅
	//todo only alpha char ✅ 
	//todo check bounds ✅
	//! CAREFUL
	cout << "parseErrorPage successful ✅\nerror_pages are\n";
	printMap(this->_config->error_pages);
}

void ServerBlock::parseClientMaxBodySize(vector<string> val)
{
	int		bytes_multiplier = 1024;
	string	err_msg = qString("client_max_body_size") + " directive invalid value in " + _filepath;
	size_t	first_not_of;
	size_t	bytes;

	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"client_max_body_size\" directive in ") + _filepath);

	if (isdigit(val[0].at(0)) == false)
		throw runtime_error(err_msg);

	first_not_of = val[0].find_first_not_of("0123456789");
	if (first_not_of != string::npos)
	{
		char	tmp = toupper(val[0].at(first_not_of));
		if (tmp != 'K' && tmp != 'M' && tmp != 'G')
			throw runtime_error(err_msg);
		(tmp == 'M') ? bytes_multiplier *= 1024 : bytes_multiplier *= (1024 * 1024);
		if (val[0].size() != first_not_of + 1) //* means that there is something after the unit
			throw runtime_error(err_msg);
	}
	istringstream	iss(val[0]);
	iss >> bytes;
	if (iss.fail())
		throw runtime_error("iss conv"); //todo might fail here if there is the unit so check. ok
	bytes *= bytes_multiplier;
	if (bytes == 0)
		this->_config->client_max_body_size = string::npos; //* disables the limit
	else
		this->_config->client_max_body_size = bytes;
	//todo only one argument ✅
	//todo start with num char (negative values arent accepted) ✅
	//todo ends with k, m or g (case insensitive, and just one of them) ✅
	//todo and nothing after the last alpha char (k, m, g) ✅
	//todo 0 disables the limit ✅
	//todo without units, treated as bytes ✅
	//todo check here if overflow
	//? set a maximum value (so that it doesn't exceed system limits)
	cout
	<< "parseClientMaxBodySizesuccessful ✅\n this->client_size = "
	<< this->_config->client_max_body_size << "\n";

}

ServerBlock::~ServerBlock(void) {}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(vector<string> val)
{
	//todo only one argument ✅
	//todo starts with / (but nginx doesnt throw an error when it is not)
	//? so does that cause error at the execution?

	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"root\" directive in ") + _filepath);
	this->_config.root = val.at(0);
	cout << "parseRoot ✅\n root : " << this->_config.root << '\n';

}

void LocationBlock::parseMethods(vector<string> val)
{
	//todo valid methods (GET, POST, DELETE), case insensitive ✅

	if (val.size() == 0 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"methods\" directive ") + _filepath);
	this->_config.methods.resize(0); //! just for testing purposes
	for (size_t i = 0; i < val.size(); i++)
	{
		string	tmp(val[i]); //? does the copy operator do a deep copy ?
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper); //applies toupper to each char
		if (tmp != "GET" && tmp != "POST" && tmp != "DELETE")
			throw runtime_error(METHOD_UNKNOWN + qString(val[i]) + " in \"methods\" directive\n");
		this->_config.methods.push_back(tmp);
	}
	cout << "parseMethods ✅\nmethods are\n";
	printVector(this->_config.methods);
}

void LocationBlock::parseDirectoryListing(vector<string> val)
{
	//todo format is "autoindex on" or "autoindex off" ✅
	//todo so there should just be on or off (therefore one argument only) ✅
	//! is triggered only if no index_file is found
	string	auto_index("\"auto_index\" directive\n");

	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + auto_index);

	string	tmp = val[0];
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	if (tmp != "off" && tmp != "on")
		throw runtime_error(INVALID_VALUE + qString(val[0]) + " in " + auto_index);
	cout << "parseDirectoryListing ✅\n";

}

void LocationBlock::parseIndexFile(vector<string> val)
{
	//todo empty or missing ✅
	//todo invalid characters in filename
	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"index_file\" directive\n"));
	this->_config.index_file = val.at(0);
	cout << "parseIndexFile ✅\nindex_file : " << this->_config.index_file << '\n';
}

void LocationBlock::parseCgiPath(vector<string> val)
{
	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"cgi_path\" directive\n"));
	this->_config.cgi_path = val.at(0);
	cout << "parseCgiPass✅\ncgi_path " << this->_config.cgi_path << '\n';
}

void LocationBlock::parseUploadDir(vector<string> val)
{
	if (val.size() != 1 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"upload_dir\" directive\n"));
	this->_config.upload_dir = val.at(0);
	cout << "parseUploadDir ✅\n" << this->_config.upload_dir << '\n';

}

void LocationBlock::parseHttpRedirect(vector<string> val)
{
	long	return_value;
	string	return_string = "\"return\" directive in ";

	if (val.size() == 0 || val.size() > 2 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + return_string + _filepath); //?nginx doesn't throw an error when one arg only

	if (val[0].find_first_not_of("0123465789") != string::npos)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0]) + " in " + return_string + _filepath);

	return_value = strtol(val[0].c_str(), NULL, 10);
	if (return_value < 300 || return_value > 399)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0]) + " in " + return_string + _filepath);
	this->_config.http_redirect = val.at(0);
	cout << "parseHttpRedirect ✅\nhttp_redirect = " << this->_config.http_redirect << '\n';

}

void LocationBlock::parseReturn(vector<string> val)
{
	long	return_value;

	if (val.size() == 0 || val.size() > 2 || val[0].empty() == true)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("\"return\" directive\n"));

	if (val[0].find_first_not_of("0123465789") != string::npos)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0]) + " in " + _filepath);

	return_value = strtol(val[0].c_str(), NULL, 10);
	if (return_value > 999)
		throw runtime_error(INVALID_RETURN_CODE + qString(val[0]) + " in " + _filepath);
	cout << "parseReturn ✅\n";
}


LocationBlock::~LocationBlock(void) {}
