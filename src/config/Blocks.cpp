/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:38:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/20 15:18:05 by mbecker          ###   ########.fr       */
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
		throw runtime_error("missing value for directive \"" + start->token + "\" in " + _filepath + ":" + itostr(start->line));

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
		throw runtime_error("missing value for directive \"" + (start - 1)->token + "\" in " + _filepath + ":" + itostr((start - 1)->line));
	start++;
	if (start->type != TKN_BLOCK_START)
		throw runtime_error("missing block start in " + _filepath + ":" + itostr(start->line));

	vector<Token>::iterator end = findBlockEnd(start);
	vector<Token> block_tokens(start, end + 1);

	if (block_tokens.size() < 2)
		throw runtime_error("empty block in " + _filepath + ":" + itostr((start - 1)->line));

	for (list<pair<string, vector<Token> > >::iterator it = _subblocks.begin(); it != _subblocks.end(); ++it)
	{
		if (it->first == location_value)
			throw runtime_error("duplicate location \"" + location_value + "\" in " + _filepath + ":" + itostr(start->line));
	}
	_subblocks.push_back(make_pair(location_value, block_tokens));

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
			throw runtime_error("unknown directive \"" + it->token + "\" in " + _filepath + ":" + itostr(it->line));
	}
	for (list<pair<string, vector<Token> > >::iterator it = _subblocks.begin(); it != _subblocks.end(); ++it)
		parseBlock(it->first, it->second);
		//cout << "parseBlock(" << it->first << ", " << "[vector]" << ")" << endl;
}

/************ SERVER BLOCK *************/

ServerBlock::ServerBlock(struct ServerConfig *config, string &filepath) 
	: _config(config)
{
	_filepath = filepath;
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

void ServerBlock::parseBlock(string context, vector<Token> tokens)
{
	RouteConfig route;
	tokens.erase(tokens.begin());
	tokens.pop_back();
	LocationBlock block(route, context, _filepath);
	block.process(tokens);
	_config->routes.push_back(route);
}

/************ LOCATION BLOCK *************/

LocationBlock::LocationBlock(struct RouteConfig &config, string context, string &filepath) 
	: _config(config), _context(context)
{
	_filepath = filepath;
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
	_allowed_fields["index"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseIndexFile);
	_allowed_fields["cgi_pass"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseCgiPath);
	_allowed_fields["upload_dir"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseUploadDir);
	_allowed_fields["http_redirect"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseHttpRedirect);
	_allowed_fields["return"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseReturn);

	_allowed_blocks.push_back("location");
}

void LocationBlock::parseBlock(string context, vector<Token> tokens)
{
	RouteConfig route;
	tokens.erase(tokens.begin());
	tokens.pop_back();
	LocationBlock block(route, context, _filepath);
	block.process(tokens);

	//TODO: handle context in nested location.
}
