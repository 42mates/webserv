/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_HEAD.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/27 17:09:46 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Response Request::handleHEAD()
{
	Response response;

	try
	{
		string file = getFileContent();
		response.setBody("");
		response.setCode("200");
		//response.setHeader(???);
	}
	catch(const ResponseException& e)
	{
		error_log  << "handleGET(): " << e.what() << endl;
		response = e.getResponse();
		access_log << "REQUEST DENIED 😱" << endl;
		return response;
	}
	access_log << "REQUEST ACCEPTED 😎" << endl;
	return response;
}
