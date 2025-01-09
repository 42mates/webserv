/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/09 16:38:40 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServ.hpp"

WebServ::WebServ()
{}

WebServ::~WebServ() {}

void WebServ::run(const char* arg, int &ret)
{
	std::string _config_file = (arg) ? arg : "conf/webserv.conf";

	try
	{
		_conf.parse(_config_file);
		std::cout << "All good, exiting" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		ret = 1;
	}



}