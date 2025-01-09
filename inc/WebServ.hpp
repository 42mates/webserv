/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/09 16:35:30 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "config/ServerConfig.hpp"

class WebServ
{
	private:
		ServerConfig _conf;

	public:
		WebServ();
		~WebServ();

		void run(const char* conf_file, int &ret);
};
