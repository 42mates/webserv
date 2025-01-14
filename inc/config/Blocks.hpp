/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 15:48:36 by mbecker          ###   ########.fr       */
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
		int _line_nb;                                                          // Line number where the block starts.
		std::ifstream *_infile;                                             // File stream for the configuration file.
		std::map<std::string, void (ABlock::*)(std::vector<std::string>)> _std_fields; // Allowed fields that can be found in a block.
		std::vector<std::string> _std_blocks;                               // Allowed blocks that can be found in a block.
		std::vector<std::string> _subblocks;                                // Blocks found in the current block.		

		void identifyDirectives();
		void extractBlock();
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

		void parseServerName(std::vector<std::string> val);
		void parseErrorPage(std::vector<std::string> val);
		void parseClientMaxBodySize(std::vector<std::string> val);

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
		
		void parseRoot(std::vector<std::string> val);
		void parseMethods(std::vector<std::string> val);
		void parseDirectoryListing(std::vector<std::string> val);
		void parseIndexFile(std::vector<std::string> val);
		void parseCgiPath(std::vector<std::string> val);
		void parseUploadDir(std::vector<std::string> val);
		void parseHttpRedirect(std::vector<std::string> val);
		void parseReturn(std::vector<std::string> val);

	public:
		LocationBlock(struct RouteConfig *config, int line);
		~LocationBlock();
		void parse(std::ifstream &stream);
};
