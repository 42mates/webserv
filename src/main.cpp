/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:43:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 16:09:19 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServ.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
		return (1);
	}

	WebServ webserv;
	int ret = 0;
    const char* config_file = (argc == 2) ? argv[1] : NULL;

	webserv.run(config_file, ret);

	return (ret);
}
