/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:30:39 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/12 18:53:58 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"

string itostr(int num) 
{
	ostringstream oss;
	oss << num;
	return oss.str();
}

int	countOccurrences(const string s, char c)
{
	int	count = 0;

	for (size_t i = 0; i < s.size(); i++)
		if (s.at(i) == c)
			count++;

	return (count);
}

/**
 * @brief Reads the entire content of a file into a string.
 */
string getFile(const string &path)
{
	ifstream file(path.c_str());
	if (!file.is_open() || !file.good())
		throw runtime_error("getFile: could not open file " + path);
	
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	file.close();

	return content;
}
