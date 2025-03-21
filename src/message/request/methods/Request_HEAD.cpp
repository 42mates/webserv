/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_HEAD.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/07 15:11:44 by mbecker          ###   ########.fr       */
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
		response.setStatus("200");
		//response.setHeader(???);
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << "handleGET(): " << e.what() << endl;
		response = e.getResponse();
		cout << RED << "REQUEST DENIED 😱" << NC << endl;
		return response;
	}
	cout << GREEN << "REQUEST ACCEPTED 😎" << NC << endl;
	return response;
}
