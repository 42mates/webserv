/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_processing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:44:01 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/07 17:02:47 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"

bool ABlock::isAllowedField(string token)
{
	return (_allowed_fields.find(token) != _allowed_fields.end());
}

bool ABlock::isAllowedBlock(string token)
{
	return (find(_allowed_blocks.begin(), _allowed_blocks.end(), token) != _allowed_blocks.end());
}

void ABlock::parseField(vector<ConfigToken>::iterator &start)
{
	vector<ConfigToken>::iterator end = start + 1;
	vector<ConfigToken> val;
	
	while (end != _tokens.end() && end->type == TKN_VALUE)
	{
		val.push_back(*end);
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
void ABlock::storeBlock(vector<ConfigToken>::iterator &start)
{
	string location_value = start->token;

	if (start == _tokens.end() || start->type != TKN_VALUE)
		throw runtime_error("missing value for directive \"" + (start - 1)->token + "\" in " + _filepath + ":" + itostr((start - 1)->line));
	start++;
	if (start->type != TKN_BLOCK_START)
		throw runtime_error("missing block start in " + _filepath + ":" + itostr(start->line));

	vector<ConfigToken>::iterator end = findBlockEnd(start);
	vector<ConfigToken> block_tokens(start, end + 1);

	if (block_tokens.size() < 2)
		throw runtime_error("empty block in " + _filepath + ":" + itostr((start - 1)->line));

	for (list<pair<string, vector<ConfigToken> > >::iterator it = _subblocks.begin(); it != _subblocks.end(); ++it)
	{
		if (it->first == location_value)
			throw runtime_error("duplicate location \"" + location_value + "\" in " + _filepath + ":" + itostr(start->line));
	}
	_subblocks.push_back(make_pair(location_value, block_tokens));

	start = end;
}

void ABlock::process(vector<ConfigToken> &tokens)
{
	_tokens = tokens;

	for (vector<ConfigToken>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
	{
		if (isAllowedField(it->token))
			parseField(it);
		else if (isAllowedBlock(it->token))
			storeBlock(++it);
		else
			throw runtime_error("unknown directive \"" + it->token + "\" in " + _filepath + ":" + itostr(it->line));
	}
	for (list<pair<string, vector<ConfigToken> > >::iterator it = _subblocks.begin(); it != _subblocks.end(); ++it)
		parseBlock(it->first, it->second); //pure virtual function
	
}


/************* ServerBlock *************/

/**
 * @brief Parses a server block from the given context and tokens, initializes a default route, processes the tokens, and updates the route configuration.
 * 
 * @param context The context path for the server block.
 * @param tokens A vector of tokens representing the server block configuration.
 */
void ServerBlock::parseBlock(string context, vector<ConfigToken> tokens)
{
	//remove '{' and '}' tokens
	tokens.erase(tokens.begin());
	tokens.pop_back();

	//init default route, and update path for route
	RouteConfig route;
	route.path = context;
	route.root = _config->root;
	route.index_file = _config->index_file;
	route.http_redirect = _config->http_redirect;
	
	LocationBlock block(route, _filepath);
	block.process(tokens);

	_config->routes[context] = route;
}


/************* LocationBlock *************/

RouteConfig LocationBlock::duplicateConfig(RouteConfig &config)
{
	RouteConfig new_config = config;
	new_config.methods = config.methods;
	new_config.directory_listing = config.directory_listing;
	new_config.index_file = config.index_file;
	new_config.cgi_path = config.cgi_path;
	new_config.upload_dir = config.upload_dir;
	new_config.http_redirect = config.http_redirect;

	//new_config.return_code = config.return_code;
	//new_config.return_value = config.return_value;

	return new_config;
}

/**
 * @brief Parses a location block, validates its context, and processes its tokens.
 * 
 * @param context The context string representing the location path.
 * @param tokens A vector of ConfigToken objects representing the tokens within the block.
 * 
 * @throws runtime_error if the location is outside the current location.
 */
void LocationBlock::parseBlock(string context, vector<ConfigToken> tokens)
{
	//remove '{' and '}' tokens
	tokens.erase(tokens.begin());
	tokens.pop_back();

	//check if location is inside current location
	if (context.find(_config->path) != 0)
		throw runtime_error("location \"" + context + "\" is outside location \"" + _config->path + "\" in " + _filepath + ":" + itostr(tokens.front().line));

	//get current config, and update path for subroute
	RouteConfig route = duplicateConfig(*_config);
	route.path = context;

	LocationBlock block(route, _filepath);
	block.process(tokens);

	_config->subroutes[context] = route;
}
