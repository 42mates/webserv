/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:25:20 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/08 15:04:44 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ! This file is the main header file for the webserv project.
// ! It contains all the includes and the main structure of the project.

#ifndef WEBSERV_H
# define WEBSERV_H


// ! System Librairies

# include <iostream>      // General I/O operations
# include <string>        // String operations
# include <fstream>       // File operations (configuration file)
# include <map>           // Storing configuration parameters
# include <vector>        // Storing multiple sockets
# include <sys/types.h>   // Socket functions
# include <sys/socket.h>  // Socket functions
# include <netinet/in.h>  // Socket functions
# include <arpa/inet.h>   // Socket functions
# include <unistd.h>      // Close function, fork, execve
# include <fcntl.h>       // Non-blocking sockets
# include <poll.h>        // Poll function
# include <cstring>       // String manipulation
# include <cstdlib>       // General utilities (exit, etc.)
# include <cstdio>        // Standard I/O (for CGI)
# include <cerrno>        // Error handling


// ! Custom Librairies

# include "WebServ.hpp"




#endif