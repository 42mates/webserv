/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testSockets.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:52:09 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/13 16:15:47 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.h"
#include <exception>

int					server_socket; // reference to the socket created by the system. 
int					client_socket;
// allows to interact with the socket (send, receive, bind..)
struct sockaddr_in	server_address;
struct sockaddr_in	client_address;
socklen_t			client_address_len;
// Contains the information such as IP address family (IPv4 here), port and IP address.
// Used to bind the socket, so that it can listen for incoming connections.

void	createSocket( void )
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		throw std::runtime_error("createSocket()");
	memset(&server_address, 0, sizeof(server_address));
}

void	bindSocket( void )
{
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr*)(&server_address), sizeof(server_address)) < 0)
		throw std::runtime_error("bindSocket()");

}
/**
 * @brief Tells the OS that a socket will be in a passive listening state, 
 * waiting for incoming connection requests. After that function is called,
 * the socket can use accept().
*/
void	listenSocket( void )
{
	if (listen(server_socket, 1) < 0)
		throw std::runtime_error("listenSocket()");
}

/**
 * @brief Allows a socket in a listening state to accept an incoming connection.
 * Fills the client_address' structure with the client's address' informations.
 * /!\ Blocks the program, waiting for a connection. 
 * @param server_socket (IN FUNCTION BODY) The socket the server is listening on.
 * @param client_address (IN FUNCTION BODY) The client’s address information (IP, port)
 * @return void. After the function is called, client_socket stores a fd that the server
 * will use to interact with that specific client
 * (In the project, the server will handle multiple connections, since each connexion 
 * will create a new socket).
*/
void	acceptConnection( void )
{
	client_socket = accept(server_socket, (struct sockaddr*)&server_address, &client_address_len);
	if (client_socket < 0)
		throw std::runtime_error("acceptConnection()");
}

void	receiveData( void )
{
	char	buffer[1024] = {0};
	int		n = recv(client_socket, buffer, sizeof(buffer), 0);
	if (n < 0)
		throw std::runtime_error("receiveData()");

	buffer[n] = '\0';
	std:: cout << buffer << "\n";
}

void sendData(void)
{
    const char* message = "Hello, client!";
    int n = send(client_socket, message, strlen(message), 0);
    if (n < 0)
        throw std::runtime_error("Error sending data");
}

// int main ( void )
// {
// 	try {
// 		createSocket();
// 		bindSocket();
// 		listenSocket();
// 		acceptConnection();

// 		receiveData();
// 		sendData();
// 	}
// 	catch (const std::exception&e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		if (server_socket > 0) close(server_socket);
// 		if (client_socket > 0) close(client_socket);
// 		return (1);
// 	}
// 	close(server_socket);
// 	close(client_socket);
// 	return (0);
// }