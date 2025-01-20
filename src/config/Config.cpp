/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:37 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/10 11:42:40 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Blocks.hpp"


Config::Config() 
	: _path(""), _file()
{}

Config::~Config()
{
	for (vector<ServerConfig *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
}

void Config::parse(string &config_file)
{
	_path = config_file;
	openFile();

	//while there are servers directives
	{
		// Skip comments and ws
		// meet server block: initialise a new ServerConfig node
		ServerConfig *new_serv = new ServerConfig();
		
		// parse the server block
		ServerBlock	serv_block(new_serv, 0);
		serv_block.parse(_file);
		_servers.push_back(new_serv);

		//repeat
	}



	_file.close();
}

void Config::openFile()
{
	_file.open(_path.c_str());

	if (!_file.is_open() || _file.bad())
	{
		if (_path == DEFAULT_CONFIG_FILE)
			throw runtime_error("Could not open default config file: " + _path + ": " + strerror(errno));
		else
			throw runtime_error("Could not open config file: " + _path + ": " + strerror(errno));
	}
}


