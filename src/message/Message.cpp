/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:41:00 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/31 17:42:33 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class AMessage
{
	protected:
		map<string, string> _general_header;
		
	public:
		AMessage();
		~AMessage();
};
