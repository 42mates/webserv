/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/10 13:19:56 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServ.hpp"

WebServ::WebServ()
{}

WebServ::~WebServ() {}

void WebServ::run(const char* arg, int &ret)
{
	std::string _config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;

	try
	{
		_conf.parse(_config_file);
		std::cout << "Config Parsing OK ✅" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		ret = 1;
	}

}