/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:55:36 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/03 17:17:09 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

#include "AMessage.hpp"

class Request : public AMessage
{
	private:
		string _method;
		string _uri;
		string _version;

		void parseHeaderLine(string header_line);
		void parseStartLine(string start_line);
		void parseBody(string body);

	public:
		Request();

		void parseRequest(string raw_request);
		void test();
};
