/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:35:16 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/24 12:42:09 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::initStatusLine()
{
	_status_line["100"] = "Continue";
	_status_line["101"] = "Switching Protocols";
	_status_line["200"] = "OK";
	_status_line["201"] = "Created";
	_status_line["202"] = "Accepted";
	_status_line["203"] = "Non-Authoritative Information";
	_status_line["204"] = "No Content";
	_status_line["205"] = "Reset Content";
	_status_line["206"] = "Partial Content";
	_status_line["300"] = "Multiple Choices";
	_status_line["301"] = "Moved Permanently";
	_status_line["302"] = "Found";
	_status_line["303"] = "See Other";
	_status_line["304"] = "Not Modified";
	_status_line["305"] = "Use Proxy";
	_status_line["307"] = "Temporary Redirect";
	_status_line["400"] = "Bad Request";
	_status_line["401"] = "Unauthorized";
	_status_line["402"] = "Payment Required";
	_status_line["403"] = "Forbidden";
	_status_line["404"] = "Not Found";
	_status_line["405"] = "Method Not Allowed";
	_status_line["406"] = "Not Acceptable";
	_status_line["407"] = "Proxy Authentication Required";
	_status_line["408"] = "Request Time-out";
	_status_line["409"] = "Conflict";
	_status_line["410"] = "Gone";
	_status_line["411"] = "Length Required";
	_status_line["412"] = "Precondition Failed";
	_status_line["413"] = "Request Entity Too Large";
	_status_line["414"] = "Request-URI Too Large";
	_status_line["415"] = "Unsupported Media Type";
	_status_line["416"] = "Requested Range Not Satisfiable";
	_status_line["417"] = "Expectation Failed";
	_status_line["500"] = "Internal Server Error";
	_status_line["501"] = "Not Implemented";
	_status_line["502"] = "Bad Gateway";
	_status_line["503"] = "Service Unavailable";
	_status_line["504"] = "Gateway Time-out";
	_status_line["505"] = "HTTP Version Not Supported";

	_status_line["DEFAULT"] = "Default Reason";
}

void Response::initHeaderFields()
{
	// MANDATORY
	_header["server"] = WEBSERV_PUBLIC_NAME;
	_header["date"] = getDate(); //to update before sending
	_header["allow"];
	_header["connection"];         // Required - Manages `keep-alive` and `close` for connection handling.
	_header["transfer-encoding"];  // Required if `chunked` - Manages encoding of request/response body.
	_header["content-length"];     // Required - Specifies the size of the request body
	
	// MANDATORY FOR BODY RESPONSES
	_header["content-type"];
	_header["location"];

	// UNIMPLEMENTED BUT ACCEPTED HEADER FIELDS
	_header["expect"];
	_header["host"];
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
	_header["content-encoding"];
	_header["content-language"];
	_header["content-location"];
	_header["forwarded"];
	_header["via"];
	_header["accept-ranges"];
	_header["range"];
	_header["if-range"];
	_header["content-range"];
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

Response::Response() 
	: _code("DEFAULT"), _start(0)
{
	initStatusLine();
	initHeaderFields();

	_body = ""; //expected behaviour from tester
	//setErrorBody();
}

Response::Response(string code) 
	: _code(code), _start(0)
{
	initStatusLine();
	if (_status_line.find(_code) == _status_line.end())
		throw runtime_error(string("debug: Response constructor used with invalid arg \"") + _code + "\"");
	initHeaderFields();

	_body = ""; //expected behaviour from tester
	//setErrorBody();
}

Response::Response(const Response &other) 
: _code(other._code), _body(other._body), _status_line(other._status_line), _start(other._start)
{
	_header = other._header;
}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_code = other._code;
		_header = other._header;
		_body = other._body;
		_status_line = other._status_line;
		_start = other._start;
	}
	return *this;
}

Response::~Response()
{}

string Response::headerToString()
{
	string header;
	for (map<string, string>::iterator it = _header.begin(); it != _header.end(); it++)
	{
		if (!it->second.empty())
			header += it->first + ": " + it->second + "\n";
	}
	return header;
}

string Response::addCRLF(string str)
{
	size_t pos = 0;
	while ((pos = str.find("\n", pos)) != string::npos) 
	{
		str.replace(pos, 1, CRLF);
		pos += 2;
	}
	return str;
}

void Response::print()
{
	cout << "Code: " << _code << endl;
	cout << "Header: " << headerToString() << endl;
	cout << "Body: " << _body << endl;
}
