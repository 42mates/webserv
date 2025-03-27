/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_GET.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 17:09:46 by mbecker          ###   ########.fr       */
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
		access_log << "GET request successfully processed. Response code: 200." << endl;
	}
	catch(const ResponseException& e)
	{
		error_log  << e.what() << endl;
		response = e.getResponse();
		access_log << "GET request failed. Error: " << e.what() << endl;
		return response;
	}
	return response;
}
