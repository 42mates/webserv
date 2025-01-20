/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/20 13:05:31 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../libs.h"
#include "../macros.h"

#include "Config.hpp" // Include the header file that defines ServerConfig


/**
 * @brief Base class for handling configuration blocks.
 */
class ABlock
{
	protected:
		int _line_nb;                                                          // Line number where the block starts.
		ifstream *_infile;                                             // File stream for the configuration file.

		map<string, void (ABlock::*)(vector<string>)> _std_fields; // Allowed fields that can be found in a block.
		vector<string> _std_blocks;                               // Allowed blocks that can be found in a block.
		vector<string> _subblocks;                                // Blocks found in the current block.		

		void identifyDirectives();
		void extractBlock();
		void parseField();

	public:
		virtual void parse(ifstream &stream) = 0;
		virtual ~ABlock() {};
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	private:
		struct ServerConfig *_config;


	public:
		void parseListen(vector<string> val);
		void parseServerName(vector<string> val);
		void parseErrorPage(vector<string> val);
		void parseClientMaxBodySize(vector<string> val);
		
		ServerBlock(struct ServerConfig *config, int line);
		void parse(ifstream &stream);
		~ServerBlock( void );
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	private:
		struct RouteConfig *_config;
		

	public:
		void parseRoot(vector<string> val);
		void parseMethods(vector<string> val);
		void parseDirectoryListing(vector<string> val);
		void parseIndexFile(vector<string> val);
		void parseCgiPath(vector<string> val);
		void parseUploadDir(vector<string> val);
		void parseHttpRedirect(vector<string> val);
		void parseReturn(vector<string> val);
		LocationBlock(struct RouteConfig *config, int line);
		~LocationBlock();
		void parse(ifstream &stream);
};
