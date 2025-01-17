/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/17 16:33:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"

Tokenizer::Tokenizer(vector<string> &file) 
	: _file(file), _line(0), _in_quotes(false), _quote_type('\0')
{}

/**
 * @brief Extracts the next token from the given line.
 *
 * This function processes the input line to extract the next token based on
 * various delimiters and quotes. It handles quoted strings, comments, and
 * special characters such as block delimiters and field delimiters.
 *
 * @param line The input string from which the next token is to be extracted.
 *             This string is modified in-place as tokens are extracted.
 * @param result A reference to a string where the extracted token will be stored.
 * @return The extracted token as a string.
 *
 * The function follows these rules:
 * 
 * - If the line is empty, it returns an empty string.
 * 
 * - If the line starts with a double quote (") or single quote ('), it toggles
 *   the in-quote state and sets the quote type.
 * 
 * - If in-quote state is active, it extracts the token until the matching quote
 *   is found or the end of the line is reached.
 * 
 * - If not in-quote state, it skips leading spaces and handles comments, block
 *   delimiters, and field delimiters.
 * 
 * - It extracts the token based on the first occurrence of spaces, field
 *   delimiters, block start, or quotes.
 */
string Tokenizer::getNextToken(string &line, string &result)
{
	size_t end;

	if (line.empty())
		return "";

	if (*line.begin() == D_QUOTE  || *line.begin() == S_QUOTE)
	{
		_in_quotes = !_in_quotes;
		if (_in_quotes)
			_quote_type = *line.begin();
		line.erase(0, 1);
	}

	if(_in_quotes)
	{
		end = line.find(_quote_type);
		if (end == string::npos)
			end = line.size();
		
		result += line.substr(0, end);
		line.erase(0, end);
	}
	else
	{
		line.erase(0, line.find_first_not_of(SPACES));
		if (*line.begin() == COMMENT)
		{
			line.clear();
			result.clear();
		}
		else if (*line.begin() == BLOCK_END  || *line.begin() == BLOCK_START
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

/**
 * @brief Adds a token structure to the tokenizer's list of tokens.
 *
 * @param token The string representation of the token to be added.
 * @param line The line number where the token was found.
 */
void Tokenizer::addToken(string &token, int line)
{
	Token new_token;

	new_token.token = token;
	new_token.line = line;
	if (token == string(1, BLOCK_START))
		new_token.type = TKN_BLOCK_START;
	else if (token == string(1, BLOCK_END))
		new_token.type = TKN_BLOCK_END;
	else if (token == string(1, FIELD_DELIMITER))
		new_token.type = TKN_FIELD_DELIMITER;
	else if (_tokens.size() > 0
		&& (_tokens.back().type == TKN_DIRECTIVE
		||  _tokens.back().type == TKN_VALUE))
		new_token.type = TKN_VALUE;
	else
		new_token.type = TKN_DIRECTIVE;

	//cout << GREEN << "token [" << NC << new_token.line << GREEN << "]: " << NC;
	//string type;
	//if (new_token.type == TKN_BLOCK_START)
	//	type = "(BLOCK_START):     ";
	//else if (new_token.type == TKN_BLOCK_END)
	//	type = "(BLOCK_END):       ";
	//else if (new_token.type == TKN_FIELD_DELIMITER)
	//	type = "(FIELD_DELIMITER): ";
	//else if (new_token.type == TKN_DIRECTIVE)
	//	type = "(DIRECTIVE):       ";
	//else if (new_token.type == TKN_VALUE)
	//	type = "(VALUE):           ";
	//cout << MAGENTA << type << NC;
	//cout << GREEN << "'" << NC << new_token.token << GREEN << "'" << NC << endl;

	_tokens.push_back(new_token);	
}

/**
 * @brief Tokenizes the content of the file stored in the _file member variable.
 * 
 * This function processes each line of the file, extracting tokens and handling
 * quoted strings.
 * 
 * @return A vector containing all the tokens extracted from the file.
 * 
 * @throws If a quoted string is not properly closed.
 */
vector<Token> Tokenizer::tokenize()
{
	string token;
	
	for (size_t i = _line; i < _file.size(); i++)
	{
		string line = _file[i] + '\n';

		while (line != "")
		{
			getNextToken(line, token);
			
			size_t j = i;
			while (_in_quotes && j < _file.size() - 1)
			{
				if (line.empty())
					line = _file[++j] + '\n';
				if (*line.begin() == D_QUOTE || *line.begin() == S_QUOTE)
					addToken(token, i + 1);	
				getNextToken(line, token);				
				if (j == _file.size() - 1)
					throw std::runtime_error(string("Missing closing quotes '") + _quote_type + "' in " + "[path]:" + itostr(i + 1));
			}
			i = j;
			if (!token.empty())
				addToken(token, i + 1);
		}
	}

	return _tokens;
}