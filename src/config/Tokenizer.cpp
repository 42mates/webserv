/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/15 18:34:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"

Tokenizer::Tokenizer(vector<string> &file, int line) 
	: _file(file), _line(line), _in_quotes(false) 
{}


void Tokenizer::skipSpaces(string &line)
{
	size_t pos = line.find_first_not_of(SPACES);
	line.erase(0, pos);
}

string Tokenizer::getNextToken(string &line, string &result)
{
	static size_t pos = 0;
	size_t start = pos;
	size_t end;
	
	cout << "[line]:   '" << line << "'"  << endl;
	cout << "[start]:  '" << start << "'" << endl;
	
	
	if (pos == line.size()) // if we reached the end of the line, return empty string for loop to be over.
	{
		pos = 0;
		return "";
	}

	if (_in_quotes) // if in quotes, return everything until next quote or end of line.
	{
		end = line.find_first_of(QUOTES, start);
		if (end == string::npos)
			end = line.size();
		result += line.substr(start, end - start);
		pos = end;
		return result;
	}
	else
	{
		skipSpaces(line);
		if (line[start] == D_QUOTE || line[start] == S_QUOTE)  // if the token is a quote, return " or '
		{
			_in_quotes = !_in_quotes;
			end = start + 1;
		}
		else if (line[start] == BLOCK_END) // if the token is a block end, return }
			end = start + 1;
		else
		{
			end = line.find_first_of(string(SPACES) + FIELD_DELIMITER + BLOCK_START, start);
			if (end == string::npos)
				end = line.size();
		}
	}

	pos = end; // mark the end of the token for next.
	result = line.substr(start, end - start);
	return result;
}



void Tokenizer::tokenize()
{
	cout << "Tokenizing file..." << endl;
	cout << "Line: " << _line << endl;
	cout << "File size: " << _file.size() << endl;


	
	for (size_t i = _line; i < _file.size(); i++)
	{
		string token;
		while (getNextToken(_file[i], token) != "")
		{
			cout << "GNT: " << token << endl;
			cout << endl; 
			token.clear();
		}
		
		//while (_in_quotes)
		//	getNextToken(_file[i], token);
		
		//while (!_in_quotes && token != "")
		//{
		//	getNextToken(_file[i], token);
		//	_tokens.push_back(token);
		//}
		//cout << "Added token: " << _tokens.back() << endl;
	}
		
}