/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:24 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/17 17:06:04 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"

struct Token
{
	string		token;
	TokenType	type;
	int			line;
};

/**
 * @brief Tokenizer for configuration files.
 */
class Tokenizer
{
	private:
		vector<string> _file;       // File stream for the configuration file.
		vector<Token>  _tokens;     // File stream for the configuration file.
		int            _line;       // Line number where the block starts.
		bool		   _in_quotes;  // Boolean to keep track of quotes.
		char		   _quote_type; // Delimiter for the current block.

		string getNextToken(string &line, string &result);
		void addToken(string &token, int line);

	public:
		Tokenizer(vector<string> &file);

		vector<Token> tokenize();
		
		vector<Token> getTokens();

		void print(const Token &new_token);
		void print();
		void print(vector<Token> &tokens);
		void print(vector<Token>::iterator begin, vector<Token>::iterator end);
};
