/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Blocks.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:20:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/10 17:51:14 by mbecker          ###   ########.fr       */
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
		std::vector<std::string> _fields;     // Fields that can be found in a server block.
		std::vector<std::string> _blocks;     // Blocks that can be found in a server block.
		
		/** 
		 * All directives found in the block. 
		 * Key: directive type ("block" or "field")
		 * Value: directive value (e.g., "server_name", "error_page", etc.) 
		 */
		std::vector<std::string, std::string> _directives; 

		void identifyDirectives(std::string line);
		void handleDirective(std::string line);

	public:
		virtual void parse(std::ifstream &file) = 0;	
};

/**
 * @brief Configuration block for a server.
 */
class ServerBlock : public ABlock
{
	public:
		ServerBlock(struct ServerConfig &config);
		void parse(std::ifstream &file);
};

/**
 * @brief Configuration block for a location.
 */
class LocationBlock : public ABlock
{
	public:
		LocationBlock(struct RouteConfig &config);
		void parse(std::ifstream &file);
};
