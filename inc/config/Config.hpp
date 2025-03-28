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

#include "ConfigTokenizer.hpp"


class ABlock;
class ServerBlock;
class LocationBlock;

/**
 * @brief Configuration data for a specific route in the web server (`location` block).
 */
struct RouteConfig {	
	string root;             	        // Root directory associated with the route	
	string alias;             	        // Alias directory associated with the route	
	string path;             	        // Path of the route (e.g., "/upload")	
	vector<string> methods;  	        // Accepted HTTP methods (e.g., {"GET", "POST"})	
	bool directory_listing;  	        // Enable or disable directory listing	
	vector<string> index_file;          // Default file for a directory (e.g., "index.html")
	string cgi_path;         	        // Path to the CGI program (e.g., "/usr/bin/php-cgi")	
	string upload_dir;       	        // Upload directory for sent files	
	pair<string, string> http_redirect; // HTTP redirection (e.g., "301 https://example.com")

	map<string, RouteConfig> subroutes; // List of subroutes

	RouteConfig() :
		root("tools/html"),
		alias(""),
		path("/"),
		directory_listing(false),
		cgi_path(""),
		upload_dir(""),
		http_redirect("", "")
	{
		methods.push_back("GET");
		//methods.push_back("POST");
		index_file.push_back("index.html");
	}
};

/**
 * @brief Configuration data for a specific server in the web server (`server` block).
 */
struct ServerConfig {	
	string host;                        // Server IP address (default: 0.0.0.0)	
	int port;                           // Server listening port (default: 80)	
	vector<string> server_names;        // Domain names associated with the server	
	map<string, string> error_pages;    // Error pages (key: HTTP code, value: file path)	
	size_t client_max_body_size;        // Max body size (default: 1 MB)
	string root;             		    // Root directory associated with the route
	string alias;             		    // Alias directory associated with the route
	vector<string> index_file;          // Default file for a directory (e.g., "index.html")
	pair<string, string> http_redirect; // HTTP redirection (e.g., "301 https://example.com")


	map<string, RouteConfig> routes; // List of configured routes

	ServerConfig() : 
		host("0.0.0.0"),
		port(80),
		client_max_body_size(1 * 1024 * 1024),
		root("tools/html"),
		alias(""),
		http_redirect("", "")
	{
		server_names.push_back("localhost");
		index_file.push_back("index.html");
		RouteConfig default_route;
		routes["/"] = default_route;
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
		vector<ConfigToken> _tokens;           // Tokens from the configuration file

		vector<string> getFileVector();
		bool isDuplicateServer(ServerConfig *sconfig);

	protected:
		string _path;                    // Path to the configuration file
		vector<ConfigToken>::iterator findBlockEnd(vector<ConfigToken>::iterator begin);

	public:
		Config();
		~Config();

		virtual void parse(string &config_file);

		// GETTERS
		vector<ServerConfig *>* getServers();

		ServerConfig* getBestServer(const string &host, int port, const string &server_name);
		
		static string getAllowedMethods(vector<string> &methods);
};
	
RouteConfig getBestRoute(const ServerConfig& server, const string &uri);
