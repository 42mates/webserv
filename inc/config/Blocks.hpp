/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/20 15:03:01 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../libs.h"
#include "../macros.h"

#include "Config.hpp"
#include "Tokenizer.hpp"

/**
 * @brief Base class for handling configuration blocks.
 */
class ABlock : public Config
{
	protected:
		vector<Token> _tokens;                                         // Tokens found in the current block.
		string _filepath;                                              // Path of the configuration file.

		map<string, void (ABlock::*)(vector<string>)> _allowed_fields; // Allowed fields that can be found in a block.
		vector<string> _allowed_blocks;                                // Allowed blocks that can be found in a block.

		list< pair<string, vector<Token> > > _subblocks;               // Route Blocks found in the current block.		

		bool isAllowedField(string token);
		bool isAllowedBlock(string token);
		void parseField(vector<Token>::iterator &start);
		void storeBlock(vector<Token>::iterator &start);
		virtual void parseBlock(string context, vector<Token> tokens) = 0;

	public:	
		virtual void initAllowedDirectives() = 0;	
		virtual void process(vector<Token> &tokens);	
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	private:
		struct ServerConfig *_config;

		void parseBlock(string context, vector<Token> tokens);
		void parseListen(vector<string> val);
		void parseServerName(vector<string> val);
		void parseErrorPage(vector<string> val);
		void parseClientMaxBodySize(vector<string> val);

	public:
		ServerBlock(struct ServerConfig *config, string &path);
		void initAllowedDirectives();
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	private:
		struct RouteConfig _config;
		string _context;
		
		void parseBlock(string context, vector<Token> tokens);
		void parseRoot(vector<string> val);
		void parseMethods(vector<string> val);
		void parseDirectoryListing(vector<string> val);
		void parseIndexFile(vector<string> val);
		void parseCgiPath(vector<string> val);
		void parseUploadDir(vector<string> val);
		void parseHttpRedirect(vector<string> val);
		void parseReturn(vector<string> val);

	public:
		LocationBlock(struct RouteConfig &config, string context, string &path);
		void initAllowedDirectives();
};
