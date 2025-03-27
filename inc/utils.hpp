/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:33:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 15:28:25 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

struct ServerConfig;
struct RouteConfig;
class Response;

// UTILS.CPP

string	itostr(int num);
int		countOccurrences(const string s, char c);
string	getTestRequest();
string	getDate();
string	getFile(string path);
vector< pair<string, string> > decodeURL(string body);

// CLIENT_UTILS.CPP

bool	isTimeOutReached(timeval& start, timeval& end, size_t timeout);
size_t	findBestServer(vector<ServerConfig>& servers, const string host, const string port);

// CONFIG_PRINT.CPP

void printConfig(RouteConfig &config);
void printConfig(RouteConfig &config, int indent_level);
void printConfig(ServerConfig &config);
void printConfig(std::vector<ServerConfig *> &configs);

