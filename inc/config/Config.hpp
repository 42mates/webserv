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

#include "../libs.h"
#include "../macros.h"

/**
 * @brief Configuration data for a specific route in the web server (`location` block).
 */
struct RouteConfig {
    string path;                  // Path of the route (e.g., "/upload")
    string root;                  // Root directory associated with the route
    vector<string> methods;       // Accepted HTTP methods (e.g., {"GET", "POST"})
    bool directory_listing;       // Enable or disable directory listing
    string index_file;            // Default file for a directory (e.g., "index.html")
    string cgi_path;              // Path to the CGI program (e.g., "/usr/bin/php-cgi")
    string upload_dir;            // Upload directory for sent files
    string http_redirect;         // HTTP redirection (e.g., "301 https://example.com")

    RouteConfig() :
		directory_listing(false)
	{}
};

/**
 * @brief Configuration data for a specific server in the web server (`server` block).
 */
struct ServerConfig {
    string host;                  // Server IP address (default: 0.0.0.0)
    int port;                     // Server listening port (default: 80)
    vector<string> server_names;  // Domain names associated with the server
    map<int, string> error_pages; // Error pages (key: HTTP code, value: file path)
    size_t client_max_body_size;  // Max body size (default: 1 MB)
    vector<RouteConfig> routes;   // List of configured routes

    ServerConfig() : 
		host("0.0.0.0"), 
		port(80), 
		client_max_body_size(1 * 1024 * 1024) 
	{}
};


/**
 * @brief Manages and stores the configuration settings for the web server.
 */
class Config
{
	private:
		vector<ServerConfig *> _servers; // List of configured servers
		string _path;                    // Path to the configuration file

		void openFile(std::ifstream& file);

	public:
		Config();
		~Config();

		void parse(string &config_file);
};
