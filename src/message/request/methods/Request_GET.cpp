/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_GET.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:49:26 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/14 17:02:57 by mbecker          ###   ########.fr       */
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
		response.setStatus("200");
		cout << GREEN << "GET REQUEST ACCEPTED 😎" << NC << endl;
	}
	catch(const ResponseException& e)
	{
		cerr << "debug: " << e.what() << endl;
		response = e.getResponse();
		cout << RED << "GET REQUEST DENIED 😱" << NC << endl;
		return response;
	}
	return response;
}
