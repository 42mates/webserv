/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_HEAD.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/04/01 14:48:57 by mbecker          ###   ########.fr       */
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
		cerr << "handleGET(): " << e.what() << endl;
		response = e.getResponse();
		return response;
	}
	return response;
}
