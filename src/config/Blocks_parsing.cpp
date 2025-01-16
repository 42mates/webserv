/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/16 20:33:57 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"


/*********** SERVER BLOCK ***********/

void ServerBlock::parseServerName(vector<string> val)
{
	if (val.size() == 0)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("server_name"));
	cout << "parseServerName(" << val[0] << ")" << endl;
	//todo not empty ✅
}

void ServerBlock::parseErrorPage(vector<string> val)
{
	string	error_page("error_page ");

	if (val.size() < 2)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + error_page);

	for (size_t i = 0; i < val.size(); i++)
	{
		if (i == val.size() - 1)
			//* store path here (last element is a path)
		if (val[i].find_first_not_of("0123456789") != string::npos)
			throw runtime_error(INVALID_VALUE_IN + error_page + val[i]);

		long tmp = atol(val[i].c_str());
		if (tmp < 300 || tmp > 599)
			throw runtime_error(ERROR_PAGE_OUT_OF_BOUND);
	}
	cout << "parseErrorPage(" << val[0] << ")" << endl;
	//todo at least 2 arguments ✅
	//todo only alpha char ✅ 
	//todo check bounds ✅
	//! CAREFUL
}

void ServerBlock::parseClientMaxBodySize(vector<string> val)
{
	string	client_max_body_size("client_max_body_size ");
	size_t	first_not_of;
	size_t	size_string;
	size_t	bytes;

	if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + client_max_body_size);

	if (isdigit(val[0].at(0)) == false)
		throw runtime_error(INVALID_VALUE_IN + client_max_body_size + val[0]);

	size_string = val[0].size();
	first_not_of = val[0].find_first_not_of("0123456789");
	if (first_not_of != string::npos)
	{
		char	tmp = toupper(val[0].at(first_not_of));
		if (tmp != 'K' && tmp != 'M' && tmp != 'G')
			throw runtime_error(INVALID_VALUE_IN + client_max_body_size + val[0]);
		if (size_string != first_not_of + 1) //* means that there is something after the unit
			throw runtime_error(INVALID_VALUE_IN + client_max_body_size + val[0]);
	}
	istringstream	iss(val[0]);
	iss >> bytes;
	if (iss.fail())
		throw runtime_error("iss conv");
	//todo only one argument ✅
	//todo start with num char (negative values arent accepted) ✅
	//todo ends with k, m or g (case insensitive, and just one of them) ✅
	//todo and nothing after the last alpha char (k, m, g) ✅
	//todo 0 disables the limit
	//todo without units, treated as bytes
	//? set a maximum value (so that it doesn't exceed system limits)
	cout << "parseClientMaxBodySize(" << val[0] << ")" << endl;
}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(vector<string> val)
{
	//todo only one argument ✅
	//todo starts with / (but nginx doesnt throw an error when it is not)
	//? so does that cause error at the execution?

	if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("root"));
	cout << "parseRoot(" << val[0] << ")" << endl;
}

void LocationBlock::parseMethods(vector<string> val)
{
	//todo valid methods (GET, POST, DELETE), case insensitive ✅

	if (val.size() == 0)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("methods"));

	for (size_t i = 0; i < val.size(); i++)
	{
		string	tmp(val[i]); //? does the copy operator do a deep copy ?
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper); //applies toupper to each char
		if (tmp != "GET" && tmp != "POST" && tmp != "DELETE")
			throw runtime_error(METHOD_UNKNOWN + val[i]);
	}
	cout << "parseMethods(" << val[0] << ")" << endl;
}

void LocationBlock::parseDirectoryListing(vector<string> val)
{
	//todo format is "autoindex on" or "autoindex off" ✅
	//todo so there should just be on or off (therefore one argument only) ✅
	if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("autoindex"));
	if (val[0] != "off" && val[0] != "on")
		throw runtime_error(INVALID_VALUE_IN + string("autoindex"));
	cout << "parseDirectoryListing(" << val[0] << ")" << endl;
}

void LocationBlock::parseIndexFile(vector<string> val)
{
	//todo empty or missing ✅
	//todo invalid characters in filename (check with le M)
	if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("index_file"));
	cout << "parseIndexFile(" << val[0] << ")" << endl;
}

void LocationBlock::parseCgiPath(vector<string> val)
{
	if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("cgi_pass"));
	cout << "parseCgiPath(" << val[0] << ")" << endl;
}

void LocationBlock::parseUploadDir(vector<string> val)
{
if (val.size() != 1)
		throw runtime_error(INVALID_NUMBER_OF_ARGUMENTS_IN + string("upload_dir"));
	cout << "parseUploadDir(" << val[0] << ")" << endl;
}

void LocationBlock::parseHttpRedirect(vector<string> val)
{
	cout << "parseHttpRedirect(" << val[0] << ")" << endl;
}

void LocationBlock::parseReturn(vector<string> val)
{
	cout << "parseReturn(" << val[0] << ")" << endl;
}
