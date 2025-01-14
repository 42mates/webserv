/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:10:06 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 16:45:32 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Blocks.hpp"

/*********** SERVER BLOCK ***********/

void ServerBlock::parseServerName(vector<string> val)
{
	cout << "parseServerName(" << val[0] << ")" << endl;
}

void ServerBlock::parseErrorPage(vector<string> val)
{
	cout << "parseErrorPage(" << val[0] << ")" << endl;
}

void ServerBlock::parseClientMaxBodySize(vector<string> val)
{
	cout << "parseClientMaxBodySize(" << val[0] << ")" << endl;
}


/*********** LOCATION BLOCK ***********/

void LocationBlock::parseRoot(vector<string> val)
{
	cout << "parseRoot(" << val[0] << ")" << endl;
}

void LocationBlock::parseMethods(vector<string> val)
{
	cout << "parseMethods(" << val[0] << ")" << endl;
}

void LocationBlock::parseDirectoryListing(vector<string> val)
{
	cout << "parseDirectoryListing(" << val[0] << ")" << endl;
}

void LocationBlock::parseIndexFile(vector<string> val)
{
	cout << "parseIndexFile(" << val[0] << ")" << endl;
}

void LocationBlock::parseCgiPath(vector<string> val)
{
	cout << "parseCgiPath(" << val[0] << ")" << endl;
}

void LocationBlock::parseUploadDir(vector<string> val)
{
	cout << "parseUploadDir(" << val[0] << ")" << endl;
}

void LocationBlock::parseHttpRedirect(vector<string> val)
{
	cout << "parseHttpRedirect(" << val[0] << ")" << endl;
}

void LocationBlock::parseReturn(vector<string> val)
{
	cout << "parseReturn(" << val[0] << ")" << endl;
}
