/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_GET.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:48:44 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Response Request::handleGET()
{
	Response response;
	try
	{
		string file = getFileContent();
		response.setBody(file);
		response.setCode("200");
	}
	catch(const ResponseException& e)
	{
		cerr << e.what() << endl;
		response = e.getResponse();
		cout << "handleGET(): GET request failed: " << e.what() << endl;
		return response;
	}
	return response;
}
