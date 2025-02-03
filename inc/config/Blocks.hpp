/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/03 15:19:26 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "libs.h"
#include "macros.h"

#include "Config.hpp"
#include "ConfigTokenizer.hpp"

/**
 * @brief Base class for handling configuration blocks.
 */
class ABlock : public Config
{
	protected:
		vector<ConfigToken> _tokens;                                         // Tokens found in the current block.
		string _filepath;                                              // Path of the configuration file.

		map<string, void (ABlock::*)(vector<ConfigToken>)> _allowed_fields; // Allowed fields that can be found in a block.
		vector<string> _allowed_blocks;                                // Allowed blocks that can be found in a block.

		list< pair<string, vector<ConfigToken> > > _subblocks;               // Route Blocks found in the current block.		

		bool isAllowedField(string token);
		bool isAllowedBlock(string token);
		void parseField(vector<ConfigToken>::iterator &start);
		void storeBlock(vector<ConfigToken>::iterator &start);
		virtual void parseBlock(string context, vector<ConfigToken> tokens) = 0;

	public:	
		virtual void initAllowedDirectives() = 0;	
		virtual void process(vector<ConfigToken> &tokens);
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	private:
		struct ServerConfig *_config;

		void parseRoot(vector<ConfigToken> val);
		void parseIndexFile(vector<ConfigToken> val);
		void parseReturn(vector<ConfigToken> val);

	public:
		void parseBlock(string context, vector<ConfigToken> tokens);
		void parseListen(vector<ConfigToken> val);
		void parseServerName(vector<ConfigToken> val);
		void parseErrorPage(vector<ConfigToken> val);
		void parseClientMaxBodySize(vector<ConfigToken> val);

		ServerBlock(struct ServerConfig *config, string &path);
		~ServerBlock();
		void initAllowedDirectives();
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	private:
		struct RouteConfig *_config;
		RouteConfig duplicateConfig(RouteConfig &config);

	public:		
		void parseBlock(string context, vector<ConfigToken> tokens);
		void parseRoot(vector<ConfigToken> val);
		void parseMethods(vector<ConfigToken> val);
		void parseDirectoryListing(vector<ConfigToken> val);
		void parseIndexFile(vector<ConfigToken> val);
		void parseCgiPath(vector<ConfigToken> val);
		void parseUploadDir(vector<ConfigToken> val);
		void parseHttpRedirect(vector<ConfigToken> val);
		void parseReturn(vector<ConfigToken> val);

		LocationBlock(struct RouteConfig &config, string &path);
		~LocationBlock();
		void initAllowedDirectives();
};
