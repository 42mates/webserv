/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:38:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 16:48:10 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"

/************ ABSTRACT DEFINITIONS *************/

void ABlock::parseField()
{
	string key;
	vector<string> val;

	key = "server_name";
	string example = "localhost";
	val.push_back(example);

	(this->*_std_fields[key])(val);
}


void ABlock::extractBlock()
{
	int depth = 0;
	string block;
	string line;
	
	//this function must start right after the bracket of a valid block.
	while (getline(*_infile, line))
	{
		

		
		_line_nb++;
	}
}

void ABlock::identifyDirectives()
{

	//"nginx: [emerg] duplicate location "/" in /home/mbecker/nginx/conf/nginx.conf:15"

	//do line by line to keep track !

	//scan first token.
		//FIELD
			//parseField()
		//BLOCK
			//extractBlock() - store everything until next "}" in a string (keep count of "{" to avoid conflicting blocks). 
			//extractBlock in queue !!
			//store str in _directives as "block".
		//UNKNOWN
			//unknown directive "[token]" in [stream]:8
		//progress in `stream` to avoid repeating the same line.

}


/************ SERVER BLOCK *************/

ServerBlock::ServerBlock(struct ServerConfig *config, int line)
{
	_config = config;
	_line_nb = line;

	_std_fields["server_name"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseServerName);
	_std_fields["error_page"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseErrorPage);
	_std_fields["client_max_body_size"] = static_cast<void (ABlock::*)(vector <string>)>(&ServerBlock::parseClientMaxBodySize);

	_std_blocks.push_back("location");
}

void ServerBlock::parse(ifstream &stream)
{
	_infile = &stream;

	parseField();
}


/************ LOCATION BLOCK *************/

LocationBlock::LocationBlock(struct RouteConfig *config, int line)
{
	_config = config;
	_line_nb = line;
	
    _std_fields["root"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseRoot);
    _std_fields["methods"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseMethods);
    _std_fields["directory_listing"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseDirectoryListing);
    _std_fields["index_file"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseIndexFile);
    _std_fields["cgi_path"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseCgiPath);
    _std_fields["upload_dir"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseUploadDir);
    _std_fields["http_redirect"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseHttpRedirect);
    _std_fields["return"] = static_cast<void (ABlock::*)(vector <string>)>(&LocationBlock::parseReturn);

	_std_blocks.push_back("location");
}

void LocationBlock::parse(ifstream &stream)
{
	(void)stream;
}


