/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:37 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/09 16:23:09 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/config/ServerConfig.hpp"

ServerConfig::ServerConfig()
	: _config_file("")
{}

ServerConfig::~ServerConfig()
{}

void ServerConfig::parse(std::string &config_file)
{
	_config_file = config_file;
	parseFile();
}

void ServerConfig::parseFile()
{
	std::ifstream file(_config_file.c_str());

	if (!file.is_open())
		throw std::runtime_error("Could not open config file: " + _config_file + ": " + strerror(errno));

	file.close();
}


