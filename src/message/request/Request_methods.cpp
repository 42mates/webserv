/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_methods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/12 19:36:55 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Response Request::handleGet()
{
	Response response;

	try
	{
		string file = getFile(_uri);
		response.setBody(file);
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleGet(): " << e.what() << endl;
		response = e.getResponse();
	}

	return response;
}

Response Request::handlePost()
{
	Response response;

	return response;
}

Response Request::handleDelete()
{
	Response response;

	return response;
}