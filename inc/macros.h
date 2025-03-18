/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:38:27 by mbecker           #+#    #+#             */
/*   Updated: 2025/03/18 18:09:21 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

using namespace std;
extern volatile int run_server; //used for the main loop (WebServ.cpp)

/************ CONFIG ************/

# define WEBSERV_PUBLIC_NAME "webserv"

# define DEFAULT_CONFIG_FILE "tools/config/webserv.conf"
# define DEFAULT_HOST "0.0.0.0"
# define DEFAULT_PORT "8080"

# define BLOCK_START '{'
# define BLOCK_END '}'
# define FIELD_DELIMITER ';'
# define COMMENT '#'
# define QUOTES "\"'"
# define S_QUOTE '\''
# define D_QUOTE '"'
# define SPACES " \t\r\n"
# define CRLF "\r\n"

enum ConfigTokenType
{
	TKN_BLOCK_START,
	TKN_BLOCK_END,
	TKN_FIELD_DELIMITER,
	TKN_DIRECTIVE,
	TKN_VALUE,
};


/************ ERRORS ************/

//--> SocketManager.cpp
# define PORT_ALREADY_IN_USE "port already in use"

# define SOCKET_CREATION_ERROR "unable to create socket's fd"
# define SOCKET_BINDING_ERROR "unable to bind socket"
# define SOCKET_LISTENING_ERROR "unable to set one socket to a listening state"

//--> Blocks_parsing.cpp
# define INVALID_NUMBER_OF_ARGUMENTS_IN "invalid number of arguments in "
# define INVALID_VALUE_IN "invalid value in "
# define HOST_NOT_FOUND "host not found in "

# define INVALID_PARAMETER "invalid parameter "
# define INVALID_VALUE "invalid value "
# define INVALID_RETURN_CODE "invalid return code "
# define INVALID_PORT "invalid port in "
# define INVALID_IP "invalid ip in "
# define METHOD_UNKNOWN "unknown method "

//--> client_handler.cpp
# define BLOCKING_OPERATION EWOULDBLOCK

# define MCS_REQUEST_TIMEOUT 10000000
# define MCS_RESPONSE_TIMEOUT 10000000

/************ COLORS ************/

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define GREEN "\033[0;32m"
# define BGREEN "\033[1;32m"
# define YELLOW "\033[0;33m"
# define BYELLOW "\033[1;33m"
# define BLUE "\033[0;34m"
# define BBLUE "\033[1;34m"
# define MAGENTA "\033[0;35m"
# define BMAGENTA "\033[1;35m"
# define CYAN "\033[0;36m"
# define BCYAN "\033[1;36m"
# define GREY "\033[0;90m"
# define NC "\033[0m"

#endif