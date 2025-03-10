/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_POST.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/10 12:09:16 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// namespace used like static for functions.
namespace {
	typedef struct POSTBody
	{
		map<string, string>	headers;
		string				content;
	} POSTBody;
	
	typedef struct POSTData 
	{
		string				type;     // Content-Type
		map<string, string>	settings; // Content-Type settings (boundary, charset, etc.)
		vector<POSTBody>	bodies;
	} POSTData;
}

static void parseContentType(string ct_header_val, POSTData &data)
{
	istringstream iss(ct_header_val);
	string tmp; 
	
	getline(iss, tmp, ' ');
	if (tmp[tmp.size() - 1] == ';')
		tmp.erase(tmp.size() - 1);
	data.type = tmp;

	while (getline(iss, tmp))
	{
		size_t pos = tmp.find("=");
		if (pos == string::npos)
			throw ResponseException(Response("400"), "invalid content-type header value");
		string key = tmp.substr(0, pos);
		string value = tmp.substr(pos + 1);
		data.settings[key] = value;
	}
}

void parseBodies(string body, POSTData &data)
{
	string boundary = "--" + data.settings["boundary"];
	size_t pos = 0;
	size_t next_pos = 0;
	size_t boundary_len = boundary.size();
	size_t content_len = body.size();
	
	cerr << pos << " " << content_len << endl;
	while (pos < content_len)
	{
		next_pos = body.find(boundary, pos);
		if (next_pos == string::npos)
			next_pos = content_len;
		POSTBody post_body;
		post_body.content = body.substr(pos, next_pos - pos);
		data.bodies.push_back(post_body);
		pos = next_pos + boundary_len;
	}
}

Response Request::handlePOST()
{
	Response response;
	POSTData data;

	parseContentType(_header["content-type"], data);

	parseBodies(_body, data);

	for (size_t i = 0; i < data.bodies.size(); i++)
	{
		POSTBody body = data.bodies[i];
		cout << string("\033[0;3") + char((i % 6) + 32)  + "m";
		cout << "Body " << i << ":\n";
		cout << body.content << endl;
		cout << NC;
	}
	
	response = Response("200");
	return response;
}

