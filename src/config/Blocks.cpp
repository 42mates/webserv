/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:38:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/18 16:54:44 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"

/************ ABSTRACT DEFINITIONS *************/

bool ABlock::isAllowedField(string token)
{
	return (_allowed_fields.find(token) != _allowed_fields.end());
}

bool ABlock::isAllowedBlock(string token)
{
	return (find(_allowed_blocks.begin(), _allowed_blocks.end(), token) != _allowed_blocks.end());
}

void ABlock::parseField(vector<Token>::iterator &start)
{
	vector<Token>::iterator end = start + 1;
	vector<string> val;
	
	while (end != _tokens.end() && end->type == TKN_VALUE)
	{
		
		val.push_back(end->token);
		end++;
	}
	if (val.empty())
		throw runtime_error("missing value for directive \"" + start->token + "\" in " + _path + ":" + itostr(start->line));

	(this->*_allowed_fields[start->token])(val);

	start = end;
}

/**
 * @brief Stores a block of tokens starting from a given iterator.
 * 
 * @param start An iterator pointing to first iterator after the given directive (location).
 */
void ABlock::storeBlock(vector<Token>::iterator &start)
{
	string location_value = start->token;

	if (start == _tokens.end() || start->type != TKN_VALUE)
		throw runtime_error("missing value for directive \"" + (start - 1)->token + "\" in " + _path + ":" + itostr((start - 1)->line));
	start++;
	if (start->type != TKN_BLOCK_START)
		throw runtime_error("missing block start in " + _path + ":" + itostr(start->line));

	vector<Token>::iterator end = findBlockEnd(start);
	vector<Token> block_tokens(start, end + 1);

	if (block_tokens.size() < 2)
		throw runtime_error("empty block in " + _path + ":" + itostr((start - 1)->line));

	_subblocks.push_back(block_tokens);

	start = end;
}

void ABlock::process(vector<Token> &tokens)
{
	_tokens = tokens;

	for (vector<Token>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
	{
		if (isAllowedField(it->token))
			parseField(it);
		else if (isAllowedBlock(it->token))
			storeBlock(++it);
		else
			throw runtime_error("unknown directive \"" + it->token + "\" in " + _path + ":" + itostr(it->line));

	}
}

/************ SERVER BLOCK *************/

ServerBlock::ServerBlock(struct ServerConfig *config, string &path) 
	: _config(config)
{
	_path = path;
	initAllowedDirectives();
}

/** 
 * Allowed fields:
 * - `server_name`
 * - `error_page`
 * - `client_max_body_size`
 * 
 * Allowed blocks:
 * - `location`
*/
void ServerBlock::initAllowedDirectives()
{
	_allowed_fields["listen"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseListen);
	_allowed_fields["server_name"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseServerName);
	_allowed_fields["error_page"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseErrorPage);
	_allowed_fields["client_max_body_size"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseClientMaxBodySize);

	_allowed_blocks.push_back("location");
}

/************ LOCATION BLOCK *************/

LocationBlock::LocationBlock(struct RouteConfig *config, string &path) 
	: _config(config)
{
	_path = path;
	initAllowedDirectives();
}

/** 
 * Allowed fields:
 * - `root`
 * - `methods`
 * - `directory_listing`
 * - `index_file`
 * - `cgigetPath()`
 * - `upload_dir`
 * - `http_redirect`
 * - `return`
 * 
 * Allowed blocks:
 * - `location`
*/
void LocationBlock::initAllowedDirectives()
{
	_allowed_fields["root"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseRoot);
	_allowed_fields["methods"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseMethods);
	_allowed_fields["directory_listing"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseDirectoryListing);
	_allowed_fields["index_file"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseIndexFile);
	_allowed_fields["cgigetPath()"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseCgiPath);
	_allowed_fields["upload_dir"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseUploadDir);
	_allowed_fields["http_redirect"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseHttpRedirect);
	_allowed_fields["return"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseReturn);

	_allowed_blocks.push_back("location");
}


