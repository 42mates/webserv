/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigConfigTokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:24 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/18 16:38:34 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"

struct ConfigToken
{
	string		token;
	ConfigTokenType	type;
	int			line;
};

/**
 * @brief ConfigTokenizer for configuration files.
 */
class ConfigTokenizer
{
	private:
		vector<string> _file;       // File stream for the configuration file.
		vector<ConfigToken>  _tokens;     // File stream for the configuration file.
		int            _line;       // Line number where the block starts.
		bool		   _in_quotes;  // Boolean to keep track of quotes.
		char		   _quote_type; // Delimiter for the current block.

		string getNextToken(string &line, string &result);
		void addToken(string &token, int line);

	public:
		ConfigTokenizer(vector<string> &file);

		vector<ConfigToken> tokenize();

		void print();
		static void print(const ConfigToken &token);
		static void print(vector<ConfigToken> &tokens);
		static void print(vector<ConfigToken>::iterator begin, vector<ConfigToken>::iterator end);
};
