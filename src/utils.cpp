/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:30:39 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/27 15:50:29 by mbecker          ###   ########.fr       */
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

string getTestRequest()
{
	string input;
	string filepath;
	cout << "Enter the path to the file to test, or empty for default (tools/message/request/valid/simple.txt): ";
	getline(cin, filepath);
	if (filepath.empty())
		filepath = "tools/message/request/valid/simple.txt";
	ifstream file(filepath.c_str());
	for (string tmp; getline(file, tmp);)
		input += tmp + "\r\n";
	file.close();

	return input;
}
