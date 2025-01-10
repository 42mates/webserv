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

#include "../../inc/config/Config.hpp"
#include "../../inc/config/Blocks.hpp"


Config::Config() 
	: _path(""), _file()
{}

Config::~Config()
{}

void Config::parse(std::string &config_file)
{
	_path = config_file;
	openFile();

	{
		// Skip comments and ws
		// meet server block: initialise a new ServerConfig node
		ServerConfig new_serv;
		
		// parse the server block
		ServerBlock	serv_block(new_serv);
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
			throw std::runtime_error("Could not open default config file: " + _path + ": " + strerror(errno));
		else
			throw std::runtime_error("Could not open config file: " + _path + ": " + strerror(errno));
	}
}


