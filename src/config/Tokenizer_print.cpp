/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer_print.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:27:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/17 17:06:18 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"

void Tokenizer::print()
{
	for (size_t i = 0; i < _tokens.size(); ++i)
		print(_tokens[i]);
}

void Tokenizer::print(vector<Token> &tokens)
{
	for (size_t i = 0; i < tokens.size(); ++i)
		print(tokens[i]);
}

void Tokenizer::print(vector<Token>::iterator begin, vector<Token>::iterator end)
{
	for (vector<Token>::iterator it = begin; it != end; ++it)
		print(*it);
}

void Tokenizer::print(const Token &new_token)
{
	cout << GREEN << "token [" << NC << new_token.line << GREEN << "]: " << NC;
	string type;
	if (new_token.type == TKN_BLOCK_START)
		type = "(BLOCK_START):     ";
	else if (new_token.type == TKN_BLOCK_END)
		type = "(BLOCK_END):       ";
	else if (new_token.type == TKN_FIELD_DELIMITER)
		type = "(FIELD_DELIMITER): ";
	else if (new_token.type == TKN_DIRECTIVE)
		type = "(DIRECTIVE):       ";
	else if (new_token.type == TKN_VALUE)
		type = "(VALUE):           ";
	cout << MAGENTA << type << NC;
	cout << GREEN << "'" << NC << new_token.token << GREEN << "'" << NC << endl;
}
