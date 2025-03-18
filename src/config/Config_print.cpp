/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_print.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:14:12 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/18 16:12:35 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/**
 * @brief Prints the configuration details of a given route.
 * 
 * @param config The RouteConfig object containing the route configuration details.
 */
void	printConfig(RouteConfig &config)
{
	cout << "****** ROUTE ******" << endl;
	cout << "Path:              " << config.path << endl;
	cout << "Root:              " << config.root << endl;
	cout << "Alias:             " << config.alias << endl;
	cout << "Methods:           ";
	for (vector<string>::iterator it = config.methods.begin(); it != config.methods.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << "Directory listing: " << (config.directory_listing ? "on" : "off") << endl;
	cout << "Index file:        " << config.index_file[0] << endl;
	cout << "CGI path:          " << config.cgi_path << endl;
	cout << "Upload directory:  " << config.upload_dir << endl;
	cout << "HTTP redirect:     " << config.http_redirect.first << " " << config.http_redirect.second << endl;

	for (map<string, RouteConfig>::iterator it = config.subroutes.begin(); it != config.subroutes.end(); it++)
	{
		printConfig(it->second, 1);
		cout << "~~~~~~~~~~" << endl;
	}
	if (config.subroutes.size() == 0)
		cout << "No subroutes" << endl;

	cout << NC;
}

/**
 * @brief Prints the configuration details of a RouteConfig object with indentation and color.
 * 
 * @param config The RouteConfig object to be printed.
 * @param indent_level The level of indentation for nested subroutes.
 */
void	printConfig(RouteConfig &config, int indent_level)
{
	string il = "";
	for (int i = 0; i < indent_level; i++)
		il += "  ";

	//adding colors
	cout << "\033[0;3" << string(1, (indent_level % 6) + 48) << "m";

	cout << il << "Path:              " << config.path << endl;
	cout << il << "Root:              " << config.root << endl;
	cout << il << "Alias:             " << config.alias << endl;
	cout << il << "Methods:           ";
	for (vector<string>::iterator it = config.methods.begin(); it != config.methods.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << il << "Directory listing: " << (config.directory_listing ? "on" : "off") << endl;
	cout << il << "Index files:       ";
	for (vector<string>::iterator it = config.index_file.begin(); it != config.index_file.end(); it++)
		cout << *it << " ";
	cout << endl;	cout << il << "CGI path:          " << config.cgi_path << endl;
	cout << il << "Upload directory:  " << config.upload_dir << endl;
	cout << il << "HTTP redirect:     " << config.http_redirect.first << " " << config.http_redirect.second << endl;

	for (map<string, RouteConfig>::iterator it = config.subroutes.begin(); it != config.subroutes.end(); it++)
	{
		if (it != config.subroutes.begin())
			cout << il << "  ----------" << endl;
		printConfig(it->second, indent_level + 1);
	}
	if (config.subroutes.size() == 0)
		cout << il << "No subroutes" << endl;

	cout << NC;
}

/**
 * @brief Prints the configuration details of a server.
 * 
 * @param config Reference to the ServerConfig object containing the configuration details.
 */
void	printConfig(ServerConfig &config)
{
	cout << "Host: " << config.host << endl;
	cout << "Port: " << config.port << endl;
	cout << "Server names: ";
	for (vector<string>::iterator it = config.server_names.begin(); it != config.server_names.end(); it++)
		cout << *it << " ";
	cout << endl;
	for (map<string, RouteConfig>::iterator it = config.routes.begin(); it != config.routes.end(); it++)
	{	
		if (it != config.routes.begin())
			cout << "  ----------" << endl;
		printConfig(it->second, 1);
	}
	cout << endl;
}

/**
 * @brief Prints the configuration details of each server in the provided vector.
 * @param configs A vector of pointers to ServerConfig objects to be printed.
 */
void	printConfig(vector<ServerConfig *> &configs)
{
	for (vector<ServerConfig *>::iterator it = configs.begin(); it != configs.end(); it++)
	{
		cout << "****** SERVER ******" << endl;
		printConfig(**it);
		cout << endl;
	}
}
