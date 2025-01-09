/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:32 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/09 16:37:49 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../libs.h"

class ServerConfig
{
	private:
		std::string _config_file;

		void parseFile();

	public:
		ServerConfig();
		~ServerConfig();

		void parse(std::string &config_file);
};
