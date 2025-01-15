/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:24 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/15 17:49:49 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"

/**
 * @brief Tokenizer for configuration files.
 */
class Tokenizer
{
	private:
		vector<string> _file;      // File stream for the configuration file.
		vector<string> _tokens;    // File stream for the configuration file.
		int            _line;      // Line number where the block starts.
		bool		   _in_quotes; // Boolean to keep track of quotes.

		void skipSpaces(string &line);
		string getNextToken(string &line, string &result);

	public:
		Tokenizer(vector<string> &file, int line);

		void tokenize();
		//vector<string> getTokens();
		
};
