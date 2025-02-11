/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/11 15:23:05 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"
#include "Config.hpp"
#include "Request.hpp"

class WebServ
{
	private:
		Config _conf;
		Response manageRequest(string raw_request, ServerConfig &server_config);

	public:
		WebServ();
		~WebServ();

		void run(const char* conf_file, int &ret);
};
