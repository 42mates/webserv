/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:45:48 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/08 14:48:24 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

class WebServ
{
	private:
		/* data */
	public:
		WebServ();
		~WebServ();

		int run();
};