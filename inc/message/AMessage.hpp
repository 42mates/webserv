/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:34:23 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/10 16:10:17 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.h"

/*
 * @brief A base class for a HTTP message, either request or response.
 */
class AMessage {
	protected:
		map<string, string> _header; // Message header fields and their values
		string              _body;   // Message body, if specified

	public:
		AMessage();

};
