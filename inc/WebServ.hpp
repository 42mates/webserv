/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/28 13:21:50 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"
#include "macros.h"
#include "Config.hpp"
#include "Request.hpp"
#include "SocketManager.hpp"

class WebServ
{
	private:
		Config _conf;
		void handleAction();

	public:
		WebServ();
		~WebServ();

		void run(const char* conf_file, int &ret);
};
