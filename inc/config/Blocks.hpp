/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/13 17:50:19 by mbecker          ###   ########.fr       */
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
		int _line;                                                          // Line number where the block starts.
		std::ifstream *_stream;                                             // File stream for the configuration file.
		std::map<std::string, void (ABlock::*)(std::string &)> _std_fields; // Allowed fields that can be found in a block.
		std::vector<std::string> _std_blocks;                               // Allowed blocks that can be found in a block.
		std::vector<std::string> _subblocks;                                // Blocks found in the current block.		

		void identifyDirectives();
		void parseField();

	public:
		virtual void parse(std::ifstream &stream) = 0;	
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	private:
		struct ServerConfig *_config;

		void parseServerName(std::string &str);
		void parseErrorPage(std::string &str);
		void parseClientMaxBodySize(std::string &str);

	public:
		ServerBlock(struct ServerConfig *config, int line);
		void parse(std::ifstream &stream);
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	private:
		struct RouteConfig *_config;
		
		void parseRoot(std::string &str);
		void parseMethods(std::string &str);
		void parseDirectoryListing(std::string &str);
		void parseIndexFile(std::string &str);
		void parseCgiPath(std::string &str);
		void parseUploadDir(std::string &str);
		void parseHttpRedirect(std::string &str);
		void parseReturn(std::string &str);

	public:
		LocationBlock(struct RouteConfig *config, int line);
		~LocationBlock();
		void parse(std::ifstream &stream);
};
