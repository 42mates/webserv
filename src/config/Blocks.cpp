/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:38:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/24 18:04:01 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"

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
	_allowed_fields["listen"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseListen);
	_allowed_fields["server_name"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseServerName);
	_allowed_fields["error_page"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseErrorPage);
	_allowed_fields["client_max_body_size"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseClientMaxBodySize);

	//default route fields
	_allowed_fields["root"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseRoot);
	_allowed_fields["index"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseIndexFile);
	_allowed_fields["return"] = static_cast<void (ABlock::*)(vector <Token>)>(&ServerBlock::parseReturn);

	_allowed_blocks.push_back("location");
}


/************ LOCATION BLOCK *************/

LocationBlock::LocationBlock(struct RouteConfig &config, string &filepath) 
	: _config(&config)
{
	_filepath = filepath;
	_config->subroutes.clear();
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
	_allowed_fields["root"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseRoot);
	_allowed_fields["methods"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseMethods);
	_allowed_fields["directory_listing"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseDirectoryListing);
	_allowed_fields["index"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseIndexFile);
	_allowed_fields["cgi_pass"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseCgiPath);
	_allowed_fields["upload_dir"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseUploadDir);
	_allowed_fields["http_redirect"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseHttpRedirect);
	_allowed_fields["return"] = static_cast<void (ABlock::*)(vector <Token>)>(&LocationBlock::parseReturn);

	_allowed_blocks.push_back("location");
}
