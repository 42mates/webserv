/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/24 15:09:49 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"

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

		map<string, void (ABlock::*)(vector<Token>)> _allowed_fields; // Allowed fields that can be found in a block.
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

	public:
		void parseBlock(string context, vector<Token> tokens);
		void parseListen(vector<Token> val);
		void parseServerName(vector<Token> val);
		void parseErrorPage(vector<Token> val);
		void parseClientMaxBodySize(vector<Token> val);

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
		void parseBlock(string context, vector<Token> tokens);
		void parseRoot(vector<Token> val);
		void parseMethods(vector<Token> val);
		void parseDirectoryListing(vector<Token> val);
		void parseIndexFile(vector<Token> val);
		void parseCgiPath(vector<Token> val);
		void parseUploadDir(vector<Token> val);
		void parseHttpRedirect(vector<Token> val);
		void parseReturn(vector<Token> val);

		LocationBlock(struct RouteConfig &config, string &path);
		~LocationBlock();
		void initAllowedDirectives();
};
