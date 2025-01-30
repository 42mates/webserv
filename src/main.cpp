/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:43:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/30 18:14:50 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServ.hpp"
#include "libs.h"
#include "SocketManager.hpp"

 int main(int argc, char **argv)
 {
 	if (argc > 2)
 	{
 		cerr << "Usage: " << argv[0] << " [configuration file]" << endl;
 		return (1);
 	}

 	WebServ webserv;
 	int ret = 0;
     const char* config_file = (argc == 2) ? argv[1] : NULL;

 	webserv.run(config_file, ret);

 	return (ret);
 }
