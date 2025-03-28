/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:33:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/28 18:05:31 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

struct ServerConfig;
struct RouteConfig;
class Response;
class Request;

// UTILS.CPP

string	itostr(int num);
int		countOccurrences(const string s, char c);
string	getTestRequest();
string	getDate();
string	getFile(string path);
vector< pair<string, string> > decodeURL(string body);


// CLIENT_UTILS.CPP

bool	isTimeOutReached(timeval& start, timeval& end, size_t timeout);
ssize_t	findBestServer(vector<ServerConfig>& servers, Request& request, bool reading_operation);
void	findClientMaxBodySize(vector<ServerConfig>& servers, Request& request, bool& size_set_to_default, size_t& c_mbs);


// CONFIG_PRINT.CPP

void printConfig(RouteConfig &config);
void printConfig(RouteConfig &config, int indent_level);
void printConfig(ServerConfig &config);
void printConfig(std::vector<ServerConfig *> &configs);

