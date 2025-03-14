/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:30:39 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/14 14:26:52 by mbecker          ###   ########.fr       */
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
		input += tmp + CRLF;
	file.close();

	return input;
}

/**
 * @brief Get the current date and time in GMT as a HTTP-date formatted string.
 * 
 * @return The current date and time in GMT, formatted as `Day, DD Mon YYYY HH:MM:SS GMT`.
 */
string getDate()
{
	time_t now = time(0);
	tm *gmtm = gmtime(&now);
	char buf[80];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtm);
	return string(buf);
}

string getFile(string path)
{
	ifstream file(path.c_str());
	if (!file.is_open() || !file.good())
		throw runtime_error("getFile(): could not open file \"" + path + "\"");
	
	string content;
	try
	{
		content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	}
	catch(const std::exception& e)
	{
		runtime_error("getFile(): could not get file \"" + path + "\"" + string(e.what()));
	}

	file.close();

	return content;
}

/**
 * @brief Checks if a timeout has been reached between two timeval structs.
 * 
 * This function calculates the time difference in microseconds between two timeval structs
 * and compares it to a given timeout value.
 * 
 * @param start The starting timeval struct.
 * @param end The ending timeval struct.
 * @param timeout The timeout value in microseconds.
 * @return True if the timeout has been reached, false otherwise.
 */
bool	isTimeOutReached(timeval& start, timeval& end, size_t timeout)
{
	size_t	start_time_ms = start.tv_sec * 1000000 + start.tv_usec;
	size_t	end_time_ms = end.tv_sec * 1000000 + end.tv_usec;

	if ((end_time_ms - start_time_ms) >= timeout)
		return (true);
	return (false);
}