/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:41:00 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/07 15:41:28 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"

AMessage::AMessage()
{
	_header["connection"];         // Required - Manages `keep-alive` and `close` for connection handling.
	_header["date"];               // Recommended - Specifies the date of the HTTP response.
	_header["transfer-encoding"];  // Required if `chunked` - Manages encoding of request/response body.

	// UNIMPLEMENTED HEADER FIELDS
	_header["cache-control"];    // Optional - Controls caching (not needed for webserv).
	_header["trailer"];          // Unnecessary - Used only with `Transfer-Encoding: chunked`.
	_header["upgrade"];          // Unnecessary - Used to switch protocols (`WebSocket`, `HTTP/2`).
	_header["via"];              // Unnecessary - Indicates proxies traversed (not relevant for webserv).
	_header["warning"];          // Unnecessary - Adds HTTP warnings (cache-related).
	_header["pragma"];           // Obsolete - Old cache control (not needed in HTTP/1.1).
}