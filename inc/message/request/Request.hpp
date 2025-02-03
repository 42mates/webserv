/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/31 17:38:50 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

#include "AMessage.hpp"

class Request : public AMessage
{
	private:
		void parseHeaders(const string& rawHeaders);
		map<string, string> _header;

	public:
		Request();
		~Request();

		void parseRequest(const string& rawRequest);
};
