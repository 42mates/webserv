/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:38:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/13 12:30:17 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/config/Blocks.hpp"

/************ ABSTRACT DEFINITIONS *************/

void ABlock::identifyDirectives(std::string line)
{
	//todo: once all directives are identified, if two location blocks are identical, return following:
	//"nginx: [emerg] duplicate location "/" in /home/mbecker/nginx/conf/nginx.conf:15"
	(void)line;
}

void ABlock::handleDirective(std::string line)
{
	(void)line;
}


/************ SERVER BLOCK *************/

ServerBlock::ServerBlock(struct ServerConfig *config)
{
	_config = config;
	
	_fields.push_back("server_name");
	_fields.push_back("error_page");
	_fields.push_back("client_max_body_size");

	_blocks.push_back("location");
}

void ServerBlock::parse(std::ifstream &file)
{
	(void)file;
}


/************ LOCATION BLOCK *************/

LocationBlock::LocationBlock(struct RouteConfig *config)
{
	_config = config;
	
	_fields.push_back("root");
	_fields.push_back("methods");
	_fields.push_back("directory_listing");
	_fields.push_back("index_file");
	_fields.push_back("cgi_path");
	_fields.push_back("upload_dir");
	_fields.push_back("http_redirect");
	_fields.push_back("return");

	_blocks.push_back("location");
}


void LocationBlock::parse(std::ifstream &file)
{
	(void)file;
}
