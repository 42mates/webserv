/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_getters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:37:29 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/07 13:58:05 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

vector<ServerConfig*>* Config::getServers()
{
	return &_servers;
}

ServerConfig* Config::getBestServer(const string &host, int port, const string &server_name)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i]->host == host && _servers[i]->port == port)
			return _servers[i];
	}
	cerr << "server not found: " << host << ":" << port << " (" << server_name << ")" << endl;
	return NULL;
}

/**
 * @brief Helper function to recursively find the best matching route.
 * 
 * This function searches through the provided map of routes to find the best
 * matching route for the given URI. It updates the best match and its length
 * during the recursion.
 * 
 * @param uri The request URI.
 * @param current The map of current routes to search.
 * @param best Pointer to the best matching RouteConfig (updated during recursion).
 * @param best_len Length of the best match (updated during recursion).
 */
static void getBestRouteHelper(const string& uri, map<string, RouteConfig>& current, RouteConfig*& best, size_t& best_len)
{
	if (current.empty())
		return ;

    for (map<string, RouteConfig>::iterator it = current.begin(); it != current.end(); it++) 
	{
        const string& location_path = it->first;
        if (uri.find(location_path) == 0 &&  (uri.size() == location_path.size() || uri[location_path.size()] == '/'))
		{
            if (location_path.size() > best_len)
			{
            	best = &it->second;
         		best_len = location_path.size();
            }
            getBestRouteHelper(uri, it->second.subroutes, best, best_len);
    	}
    }
}


/**
 * @brief Main function to find the best route for a given server name and URI.
 * 
 * This function finds the best matching route configuration for the specified
 * server name and URI. It first searches for a server with the matching server
 * name, then uses the helper function to find the best matching route within
 * that server's routes. If no specific match is found, it returns the default
 * route ("/") if available.
 * 
 * @param server The server config structure.
 * @param uri The request URI.
 * @return RouteConfig The best matching route configuration.
 * @throws runtime_error If no matching route or default route is found.
 */
RouteConfig getBestRoute(const ServerConfig& server, const string &uri)
{
	RouteConfig* best = NULL;
	size_t best_len = 0;

	getBestRouteHelper(uri, const_cast<map<string, RouteConfig>&>(server.routes), best, best_len);
	if (best)
		return *best;
	if (server.routes.count("/"))
		return server.routes.at("/");
	else
		return server.routes.begin()->second;
	throw runtime_error("No route found and no default route defined");
}

string Config::getAllowedMethods(vector<string> &methods)
{
	string allowed_methods = "";
	for (size_t i = 0; i < methods.size(); i++)
	{
		if (i > 0)
			allowed_methods += ", ";
		allowed_methods += methods[i];
	}
	return allowed_methods;
}
