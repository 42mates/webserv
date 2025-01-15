/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/15 16:52:46 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../libs.h"
#include "../macros.h"

/**
 * @brief Base class for handling configuration blocks.
 */
class ABlock
{
	protected:
		int            _line;                                   // Line number where the block starts.
		vector<string> _file;                                      // File stream for the configuration file.
		vector<string> _tokens;                                    // File stream for the configuration file.

		map<string, void (ABlock::*)(vector<string>)> _std_fields; // Allowed fields that can be found in a block.
		vector<string> _std_blocks;                                // Allowed blocks that can be found in a block.

		vector<string> _subblocks;                                 // Blocks found in the current block.		

		void identifyDirectives();
		void parseField();

	public:
		virtual void parse(vector<string> &file) = 0;	
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	private:
		struct ServerConfig *_config;

		void parseServerName(vector<string> val);
		void parseErrorPage(vector<string> val);
		void parseClientMaxBodySize(vector<string> val);

	public:
		ServerBlock(struct ServerConfig *config, int line);
		void parse(vector<string> &file);
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	private:
		struct RouteConfig *_config;
		
		void parseRoot(vector<string> val);
		void parseMethods(vector<string> val);
		void parseDirectoryListing(vector<string> val);
		void parseIndexFile(vector<string> val);
		void parseCgiPath(vector<string> val);
		void parseUploadDir(vector<string> val);
		void parseHttpRedirect(vector<string> val);
		void parseReturn(vector<string> val);

	public:
		LocationBlock(struct RouteConfig *config, int line);
		void parse(vector<string> &file);
};
