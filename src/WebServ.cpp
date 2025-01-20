/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:48:43 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 16:45:32 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServ.hpp"

WebServ::WebServ()
{}

WebServ::~WebServ() {}

void WebServ::run(const char* arg, int &ret)
{
	string _config_file = (arg) ? arg : DEFAULT_CONFIG_FILE;

	try
	{
		_conf.parse(_config_file);
		cout << "Config Parsing OK ✅" << endl;
	}
	catch(const exception& e)
	{
		cerr << "webserv: " << e.what() << endl;
		ret = 1;
	}

}