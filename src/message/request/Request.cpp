/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/27 15:30:48 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::initHeaderFields()
{
	_header["expect"];             // Required - Used for `100-continue` before sending request body
	_header["host"];               // Required - Specifies the target host (mandatory in HTTP/1.1)

	// UNIMPLEMENTED HEADER FIELDS
	_header["accept"];             // Optional - Specifies preferred media types (e.g., text/html, application/json)
	_header["accept-charset"];     // Optional - Specifies preferred character sets (e.g., UTF-8)
	_header["accept-encoding"];    // Optional - Specifies supported content encodings (e.g., gzip, deflate)
	_header["accept-language"];    // Optional - Specifies preferred languages for response (e.g., en-US, fr-FR)
	_header["authorization"];      // Optional - Contains credentials for HTTP authentication (e.g., Basic, Bearer)
	_header["range"];              // Optional - Used for partial content requests (e.g., video streaming)
	_header["referer"];            // Optional - Indicates the previous page (useful for logs/security)
	_header["user-agent"];         // Optional - Identifies the client (browser, bot, etc.), useful for logs
	_header["from"];               // Ignored - Rarely used, contains client’s email address
	_header["if-match"];           // Ignored - Used for conditional requests based on ETag
	_header["if-modified-since"];  // Ignored - Used for cache validation (not required in webserv)
	_header["if-none-match"];      // Ignored - Used to check if resource changed (ETag comparison)
	_header["if-range"];           // Ignored - Used for resuming downloads (Range requests)
	_header["if-unmodified-since"];// Ignored - Conditional request based on modification date
	_header["max-forwards"];       // Ignored - Used in TRACE and OPTIONS (not required in webserv)
	_header["proxy-authorization"];// Ignored - Used for authentication with a proxy server
	_header["te"];                 // Ignored - Specifies transfer encodings (not required for webserv)
}

void Request::initMethodHandling()
{
	_method_handling["GET"] = &Request::handleGet;
	_method_handling["HEAD"] = &Request::handleHead;
	_method_handling["POST"] = &Request::handlePost;
	_method_handling["DELETE"] = &Request::handleDelete;

	// UNIMPLEMENTED METHODS
	//_method_handling["PUT"] = &Request::handlePut;
	//_method_handling["CONNECT"] = &Request::handleConnect;
	//_method_handling["OPTIONS"] = &Request::handleOptions;
	//_method_handling["TRACE"] = &Request::handleTrace;
}

Request::Request()
	: _method(""), _uri(""), _version(""), _start(0)
{
	initHeaderFields();
	initMethodHandling();
}

void Request::setIsCompleteRequest(bool is_end_of_request)
{
	_is_complete_request = is_end_of_request;
}
