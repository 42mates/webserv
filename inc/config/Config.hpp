/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/10 12:49:42 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"

#include "Tokenizer.hpp"


class ABlock;
class ServerBlock;
class LocationBlock;

/**
 * @brief Configuration data for a specific route in the web server (`location` block).
 */
struct RouteConfig {	
	string root;             // Root directory associated with the route	
	string path;             // Path of the route (e.g., "/upload")	
	vector<string> methods;  // Accepted HTTP methods (e.g., {"GET", "POST"})	
	bool directory_listing;  // Enable or disable directory listing	
	string index_file;       // Default file for a directory (e.g., "index.html")	
	string cgi_path;         // Path to the CGI program (e.g., "/usr/bin/php-cgi")	
	string upload_dir;       // Upload directory for sent files	
	string http_redirect;    // HTTP redirection (e.g., "301 https://example.com")
	
	map<string, RouteConfig> subroutes; // List of subroutes

	RouteConfig() :
		root("tools/html"),
		path("/"),
		directory_listing(false),
		index_file("index.html"),
		cgi_path(""),
		upload_dir("tools/uploads"),
		http_redirect("")
	{
		methods.push_back("GET");
		methods.push_back("POST");
	}
};

/**
 * @brief Configuration data for a specific server in the web server (`server` block).
 */
struct ServerConfig {	
	string host;                     // Server IP address (default: 0.0.0.0)	
	int port;                        // Server listening port (default: 80)	
	vector<string> server_names;     // Domain names associated with the server	
	map<int, string> error_pages;    // Error pages (key: HTTP code, value: file path)	
	size_t client_max_body_size;     // Max body size (default: 1 MB)	

	map<string, RouteConfig> routes; // List of configured routes

	ServerConfig() : 
		host("0.0.0.0"),
		port(8080),
		client_max_body_size(1 * 1024 * 1024)
	{
		server_names.push_back("localhost");
		//error_pages[400] = "tools/html/error_pages/400.html";
		//error_pages[401] = "tools/html/error_pages/401.html";
		//error_pages[403] = "tools/html/error_pages/403.html";
		//error_pages[404] = "tools/html/error_pages/404.html";
	}
};

/**
 * @brief Manages and stores the configuration settings for the web server.
 */
class Config
{
	private:
		vector<string> _std_blocks;      // Allowed blocks that can be found in the configuration file
		vector<ServerConfig *> _servers; // List of configured servers
		vector<Token> _tokens;           // Tokens from the configuration file

		vector<string> getFileVector();

	protected:
		string _path;                    // Path to the configuration file
		vector<Token>::iterator findBlockEnd(vector<Token>::iterator begin);

	public:
		Config();
		~Config();

		virtual void parse(string &config_file);

		ServerConfig getServer(const string &server_name);
		RouteConfig getRoute(const ServerConfig *server, const string &uri);

};
