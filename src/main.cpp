/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:43:34 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/08 14:51:25 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.h"

int main()
{
	WebServ webserv;
	int ret;

	ret = webserv.run();
	
	return (ret);
}