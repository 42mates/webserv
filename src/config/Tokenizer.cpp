/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/16 17:29:53 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"

Tokenizer::Tokenizer(vector<string> &file, int line) 
	: _file(file), _line(line), _in_quotes(false) 
{}


//string Tokenizer::getNextToken(string &line, string &result)
//{
//	size_t end;

//	cout << MAGENTA << "[TOKEN] " << NC << "Line: " << line ;
//	cout << MAGENTA << "in quotes: " << NC << _in_quotes << endl;

//	if (line.empty())// || line == "#")
//		return "";



//	if (_in_quotes)
//	{
//		end = line.find_first_of(QUOTES);
//		if (end == string::npos)
//			end = line.size();
//		else 
//			_in_quotes = false;
		
//		result += line.substr(0, end);
//		line.erase(0, end);
//		cout << "LINE AFTER: " << line << endl;
//	}
//	else
//	{
//		//skip leading spaces
//		line.erase(0, line.find_first_not_of(SPACES));
		
//		// if the first character is a delimiter, return it.
//		if (*line.begin() == D_QUOTE || *line.begin() == S_QUOTE 
//		 || *line.begin() == BLOCK_END  || *line.begin() == BLOCK_START
//		 || *line.begin() == FIELD_DELIMITER )
//		{
//			if (*line.begin() == D_QUOTE || *line.begin() == S_QUOTE)
//				_in_quotes = !_in_quotes;
//			result = line.substr(0, 1);
//			line.erase(0, 1);
//		}
//		else
//		{
//			end = line.find_first_of(string(SPACES) + FIELD_DELIMITER + BLOCK_START);
//			if (end == string::npos)
//				end = line.size();
//			result = line.substr(0, end);
//			line.erase(0, end);
//		}
//	}

//	return result;
//}

string Tokenizer::getNextToken(string &line, string &result)
{
	size_t end;

	cout << MAGENTA << "[TOKEN] " << NC << "Line: " << line ;
	cout << MAGENTA << "in quotes: " << NC << _in_quotes << endl;

	if (line.empty())// || line == "#")
		return "";

	if (*line.begin() == D_QUOTE  || *line.begin() == S_QUOTE)
	{
		_in_quotes = !_in_quotes;
		line.erase(0, 1);
	}

	if(_in_quotes)
	{
		end = line.find_first_of(QUOTES);
		if (end == string::npos)
			end = line.size();
		
		result += line.substr(0, end);
		line.erase(0, end);
		
		cout << "LINE AFTER: " << line << endl;
	}
	else
	{
		line.erase(0, line.find_first_not_of(SPACES));
		
		// if the first character is a delimiter, return it.
		if (*line.begin() == BLOCK_END  || *line.begin() == BLOCK_START
			|| *line.begin() == FIELD_DELIMITER )
		{
			result = line.substr(0, 1);
			line.erase(0, 1);
		}
		else
		{
			end = line.find_first_of(string(SPACES) + FIELD_DELIMITER + BLOCK_START + QUOTES);
			if (end == string::npos)
				end = line.size();
			result = line.substr(0, end);
			line.erase(0, end);
		}
	}

	return result;
}



void Tokenizer::tokenize()
{
	cout << "Tokenizing file..." << endl;
	cout << "Line: " << _line << endl;
	cout << "File size: " << _file.size() << endl;
	cout << endl;


	string token;
	
	for (size_t i = _line; i < _file.size(); i++)
	{
		string line = _file[i] + '\n';

		while (line != "")
		{
			while (!_in_quotes && getNextToken(line, token) != "")
			{
				if (_in_quotes && (token == string(1, D_QUOTE) || token == string(1, S_QUOTE)))
				{
					cout << YELLOW << "GNT '" << NC << token << YELLOW << "'" << NC << endl;
					token.clear();
				}
				else
					cout << GREEN << "GNT '" << NC << token << GREEN << "'" << NC << endl;	
			}
	
			while (_in_quotes)
			{								
				if (line.empty() && i < _file.size())
					line = _file[++i] + '\n';
				getNextToken(line, token);
				if (!_in_quotes)
					cout << BLUE << "GNT '" << NC << token << BLUE << "'" << NC << endl;
			}
		}
		

		cout << GREY << "EOL" << NC << endl;

	}
	if (_in_quotes)
		throw std::runtime_error("Error: Missing closing quotes.");
		
}