/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:25:20 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/09 16:35:14 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ! This file contains all the librairies for the authorized functions in the project.

#ifndef LIBS_H
# define LIBS_H

# include <algorithm>	   // Functions operating on ranges (search, sort...)
# include <iostream>       // General I/O operations
# include <string>         // String operations
# include <sstream>        // String operations
# include <fstream>        // File operations (configuration file)
# include <algorithm>      // String operations
# include <map>            // STL
# include <list>           // STL
# include <vector>         // STL
# include <sys/wait.h>     // for wait
# include <sys/stat.h>     // for stat
# include <sys/types.h>    // Socket functions
# include <sys/socket.h>   // Socket functions
# include <netinet/in.h>   // Socket functions
# include <arpa/inet.h>    // Socket functions
# include <unistd.h>       // Close function, fork, execve
# include <fcntl.h>        // Non-blocking sockets
# include <poll.h>         // Poll function
# include <cstring>        // String manipulation
# include <cstdlib>        // General utilities (exit, etc.)
# include <cstdio>         // Standard I/O (for CGI)
# include <cerrno>         // Error handling
# include <signal.h>	   // Signals
# include "macros.h"       // Macros for the project

# include "utils.hpp"      // Utilities functions (like itostr)

#endif
