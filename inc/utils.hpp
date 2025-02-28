/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:33:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 15:30:38 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"


// UTILS.CPP

std::string itostr(int num);
int	countOccurrences(const string s, char c);
string getTestRequest();
string getDate();


// CONFIG_PRINT.CPP

struct ServerConfig;
struct RouteConfig;
void printConfig(RouteConfig &config);
void printConfig(RouteConfig &config, int indent_level);
void printConfig(ServerConfig &config);
void printConfig(std::vector<ServerConfig *> &configs);

