/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:08:21 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:15:45 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::initHeaderFields()
{
	_header["connection"];         // Required - Manages `keep-alive` and `close` for connection handling.
	_header["date"];               // Recommended - Specifies the date of the HTTP response.
	_header["transfer-encoding"];  // Required if `chunked` - Manages encoding of request/response body.
	_header["content-length"];     // Required - Specifies the size of the request body
	_header["expect"];             // Required - Used for `100-continue` before sending request body
	_header["host"];               // Required - Specifies the target host (mandatory in HTTP/1.1)

	// UNIMPLEMENTED BUT ACCEPTED HEADER FIELDS
	_header["www-authenticate"];
	_header["authorization"];
	_header["proxy-authenticate"];
	_header["proxy-authorization"];
	_header["age"];
	_header["cache-control"];
	_header["clear-site-data"];
	_header["expires"];
	_header["no-vary-search"];
	_header["last-modified"];
	_header["etag"];
	_header["if-match"];
	_header["if-none-match"];
	_header["if-modified-since"];
	_header["if-unmodified-since"];
	_header["vary"];
	_header["connection"];
	_header["keep-alive"];
	_header["accept"];
	_header["accept-encoding"];
	_header["accept-language"];
	_header["accept-patch"];
	_header["accept-post"];
	_header["expect"];
	_header["max-forwards"];
	_header["cookie"];
	_header["set-cookie"];
	_header["access-control-allow-credentials"];
	_header["access-control-allow-headers"];
	_header["access-control-allow-methods"];
	_header["access-control-allow-origin"];
	_header["access-control-expose-headers"];
	_header["access-control-max-age"];
	_header["access-control-request-headers"];
	_header["access-control-request-method"];
	_header["origin"];
	_header["timing-allow-origin"];
	_header["content-disposition"];
	_header["content-digest"];
	_header["repr-digest"];
	_header["want-content-digest"];
	_header["want-repr-digest"];
	_header["content-length"];
	_header["content-type"];
	_header["content-encoding"];
	_header["content-language"];
	_header["content-location"];
	_header["forwarded"];
	_header["via"];
	_header["accept-ranges"];
	_header["range"];
	_header["if-range"];
	_header["content-range"];
	_header["location"];
	_header["refresh"];
	_header["from"];
	_header["host"];
	_header["referer"];
	_header["referrer-policy"];
	_header["user-agent"];
	_header["allow"];
	_header["server"];
	_header["cross-origin-embedder-policy"];
	_header["cross-origin-opener-policy"];
	_header["cross-origin-resource-policy"];
	_header["content-security-policy"];
	_header["content-security-policy-report-only"];
	_header["expect-ct"];
	_header["permissions-policy"];
	_header["reporting-endpoints"];
	_header["strict-transport-security"];
	_header["upgrade-insecure-requests"];
	_header["x-content-type-options"];
	_header["x-frame-options"];
	_header["x-permitted-cross-domain-policies"];
	_header["x-powered-by"];
	_header["x-xss-protection"];
	_header["sec-fetch-site"];
	_header["sec-fetch-mode"];
	_header["sec-fetch-user"];
	_header["sec-fetch-dest"];
	_header["sec-purpose"];
	_header["service-worker-navigation-preload"];
	_header["reporting-endpoints_2"];
	_header["report-to"];
	_header["transfer-encoding"];
	_header["te"];
	_header["trailer"];
	_header["sec-websocket-accept"];
	_header["sec-websocket-extensions"];
	_header["sec-websocket-key"];
	_header["sec-websocket-protocol"];
	_header["sec-websocket-version"];
	_header["alt-svc"];
	_header["alt-used"];
	_header["date"];
	_header["link"];
	_header["retry-after"];
	_header["server-timing"];
	_header["service-worker"];
	_header["service-worker-allowed"];
	_header["sourcemap"];
	_header["upgrade"];
	_header["priority"];
	_header["attribution-reporting-eligible"];
	_header["attribution-reporting-register-source"];
	_header["attribution-reporting-register-trigger"];
	_header["accept-ch"];
	_header["critical-ch"];
	_header["sec-ch-ua"];
	_header["sec-ch-ua-arch"];
	_header["sec-ch-ua-bitness"];
	_header["sec-ch-ua-form-factors"];
	_header["sec-ch-ua-full-version"];
	_header["sec-ch-ua-full-version-list"];
	_header["sec-ch-ua-mobile"];
	_header["sec-ch-ua-model"];
	_header["sec-ch-ua-platform"];
	_header["sec-ch-ua-platform-version"];
	_header["sec-ch-ua-wow64"];
	_header["sec-ch-prefers-color-scheme"];
	_header["sec-ch-prefers-reduced-motion"];
	_header["sec-ch-prefers-reduced-transparency"];
	_header["content-dpr"];
	_header["device-memory"];
	_header["dpr"];
	_header["viewport-width"];
	_header["width"];
	_header["downlink"];
	_header["ect"];
	_header["rtt"];
	_header["save-data"];
	_header["dnt"];
	_header["tk"];
	_header["sec-gpc"];
	_header["origin-agent-cluster"];
	_header["nel"];
	_header["observe-browsing-topics"];
	_header["sec-browsing-topics"];
	_header["early-data"];
	_header["set-login"];
	_header["speculation-rules"];
	_header["supports-loading-mode"];
	_header["x-forwarded-for"];
	_header["x-forwarded-host"];
	_header["x-forwarded-proto"];
	_header["x-dns-prefetch-control"];
	_header["x-robots-tag"];
	_header["pragma"];
	_header["warning"];
}

void Request::initMethodHandling()
{
	_method_handling["GET"] = &Request::handleGET;
	_method_handling["HEAD"] = &Request::handleHEAD;
	_method_handling["POST"] = &Request::handlePOST;
	_method_handling["DELETE"] = &Request::handleDELETE;

	// UNIMPLEMENTED METHODS
	//_method_handling["PUT"] = &Request::handlePUT;
	//_method_handling["CONNECT"] = &Request::handleCONNECT;
	//_method_handling["OPTIONS"] = &Request::handleOPTIONS;
	//_method_handling["TRACE"] = &Request::handleTRACE;
}

Request::Request() :	
	_method(""),
	_uri(""),
	_version(""),
	_query(""),
	_body(""),
	_path(""),
	_body_size(0),
	_is_complete_request(false),
	_raw_request(""),
	_raw_body(""),
	_start(0),
	_header_parsed(false),
	_body_filename("")
{
	initHeaderFields();
	initMethodHandling();

	// Create a temporary file for the request body
	char body_filename[] = "/tmp/request_temp_XXXXXX";
	int fd = mkstemp(body_filename);
	if (fd == -1)
		throw runtime_error("Failed to create temporary file");
	_body_filename = body_filename;

	// Open the file stream for writing
	_body_stream.open(_body_filename.c_str(), ios::out | ios::in | ios::trunc);
	close(fd);
	if (!_body_stream.is_open())
		throw runtime_error("Failed to open temporary file stream");
}

Request::~Request()
{
	// Close the file stream and remove the temporary file
	if (_body_stream.is_open())
	{
		_body_stream.close();
	}
	if (!_body_filename.empty())
	{
		remove(_body_filename.c_str());
	}
}

/************ GETTERS ************/

size_t Request::getBodySize()
{
	return _body_stream.tellg();
}

string Request::getBodyString()
{
	string body;
	try
	{
		_body_stream.seekg(0, ios::beg);
		body.assign(istreambuf_iterator<char>(_body_stream), istreambuf_iterator<char>());
	} 
	catch (const exception &e) 
	{
		throw ResponseException(Response("500"), "Failed to assign body string: " + string(e.what()));
	}
	return body;
}

string Request::getBodyString(fstream &file)
{
	string body;
	try
	{
		if (!file.is_open() || !file.good())
			throw runtime_error("File stream is not open or in a good state");
		file.seekg(0, ios::beg);
		body.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	} 
	catch (const exception &e) 
	{
		throw ResponseException(Response("500"), "Failed to assign body string: " + string(e.what()));
	}
	return body;
}

string Request::getHeaderValue(string key)
{
	if (key == "content-type")
	{
		istringstream iss(_header["content-type"]);
		string tmp;
		getline(iss, tmp, ' ');
		if (tmp[tmp.size() - 1] == ';')
			tmp.erase(tmp.size() - 1);
		return tmp;
	}
	return (_header[key]);
}

string Request::getConnectionKeepAlive()
{
	return (_header["connection"]);
}

string Request::getRawRequest()
{
	return _raw_request;
}

bool	Request::getIsCompleteRequest()
{
	return _is_complete_request;
}


/************ SETTERS ************/
	
void	Request::setIsCompleteRequest(bool val)
{
	_is_complete_request = val;
}

