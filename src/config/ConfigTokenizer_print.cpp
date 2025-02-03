/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTokenizer_print.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/18 16:38:27 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigTokenizer.hpp"

void ConfigTokenizer::print()
{
	for (size_t i = 0; i < _tokens.size(); ++i)
		print(_tokens[i]);
}

void ConfigTokenizer::print(vector<ConfigToken> &tokens)
{
	for (size_t i = 0; i < tokens.size(); ++i)
		print(tokens[i]);
}

void ConfigTokenizer::print(vector<ConfigToken>::iterator begin, vector<ConfigToken>::iterator end)
{
	for (vector<ConfigToken>::iterator it = begin; it != end; ++it)
		print(*it);
}

void ConfigTokenizer::print(const ConfigToken &token)
{
	cout << GREEN << "token [" << NC << token.line << GREEN << "]: " << NC;
	string type;
	if (token.type == TKN_BLOCK_START)
		type = "(BLOCK_START):     ";
	else if (token.type == TKN_BLOCK_END)
		type = "(BLOCK_END):       ";
	else if (token.type == TKN_FIELD_DELIMITER)
		type = "(FIELD_DELIMITER): ";
	else if (token.type == TKN_DIRECTIVE)
		type = "(DIRECTIVE):       ";
	else if (token.type == TKN_VALUE)
		type = "(VALUE):           ";
	cout << MAGENTA << type << NC;
	cout << GREEN << "'" << NC << token.token << GREEN << "'" << NC << endl;
}
