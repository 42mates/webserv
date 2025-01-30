/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:33:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/28 14:46:18 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

//utils.cpp
std::string itostr(int num);
int	countOccurrences(const string s, char c);

/**
 * @brief Checks if two vectors share any common value and returns the index of the first common value found in the first vector.
 * 
 * @tparam T Type of elements in the vectors.
 * @param v1 First vector to check.
 * @param v2 Second vector to check.
 * @return size_t Index of the first common value in v1, or -1 if no common value is found.
 */
template <typename T>
long vectorsShareValue(vector<T> &v1, vector<T> &v2)
{
	int i = 0;
	for (typename vector<T>::iterator it = v1.begin(); it != v1.end(); it++, i++)
	{
		if (find(v2.begin(), v2.end(), *it) != v2.end())
			return i;
	}
	return -1;
}


//Config
struct ServerConfig;
struct RouteConfig;

void printConfig(RouteConfig &config);
void printConfig(RouteConfig &config, int indent_level);
void printConfig(ServerConfig &config);
void printConfig(std::vector<ServerConfig *> &configs);

