/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/13 17:57:29 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/config/Blocks.hpp"

void ServerBlock::parseServerName(std::string &str)
{
	std::cout << "parseServerName(" << str << ")" << std::endl;
}

void ServerBlock::parseErrorPage(std::string &str)
{
	std::cout << "parseErrorPage(" << str << ")" << std::endl;
}

void ServerBlock::parseClientMaxBodySize(std::string &str)
{
	std::cout << "parseClientMaxBodySize(" << str << ")" << std::endl;
}


void LocationBlock::parseRoot(std::string &str)
{
	std::cout << "parseRoot(" << str << ")" << std::endl;}

void LocationBlock::parseMethods(std::string &str)
{
	std::cout << "parseMethods(" << str << ")" << std::endl;}

void LocationBlock::parseDirectoryListing(std::string &str)
{
	std::cout << "parseDirectoryListing(" << str << ")" << std::endl;}

void LocationBlock::parseIndexFile(std::string &str)
{
	std::cout << "parseIndexFile(" << str << ")" << std::endl;}

void LocationBlock::parseCgiPath(std::string &str)
{
	std::cout << "parseCgiPath(" << str << ")" << std::endl;}

void LocationBlock::parseUploadDir(std::string &str)
{
	std::cout << "parseUploadDir(" << str << ")" << std::endl;}

void LocationBlock::parseHttpRedirect(std::string &str)
{
	std::cout << "parseHttpRedirect(" << str << ")" << std::endl;}

void LocationBlock::parseReturn(std::string &str)
{
	std::cout << "parseReturn(" << str << ")" << std::endl;}
