/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 15:53:03 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/config/Blocks.hpp"

/*********** SERVER BLOCK ***********/

void ServerBlock::parseServerName(std::vector<std::string> val)
{
	std::cout << "parseServerName(" << val[0] << ")" << std::endl;
}

void ServerBlock::parseErrorPage(std::vector<std::string> val)
{
	std::cout << "parseErrorPage(" << val[0] << ")" << std::endl;
}

void ServerBlock::parseClientMaxBodySize(std::vector<std::string> val)
{
	std::cout << "parseClientMaxBodySize(" << val[0] << ")" << std::endl;
}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(std::vector<std::string> val)
{
	std::cout << "parseRoot(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseMethods(std::vector<std::string> val)
{
	std::cout << "parseMethods(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseDirectoryListing(std::vector<std::string> val)
{
	std::cout << "parseDirectoryListing(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseIndexFile(std::vector<std::string> val)
{
	std::cout << "parseIndexFile(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseCgiPath(std::vector<std::string> val)
{
	std::cout << "parseCgiPath(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseUploadDir(std::vector<std::string> val)
{
	std::cout << "parseUploadDir(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseHttpRedirect(std::vector<std::string> val)
{
	std::cout << "parseHttpRedirect(" << val[0] << ")" << std::endl;
}

void LocationBlock::parseReturn(std::vector<std::string> val)
{
	std::cout << "parseReturn(" << val[0] << ")" << std::endl;
}
