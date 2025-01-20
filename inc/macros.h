/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:38:27 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/20 15:31:13 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

using namespace std;

/************ CONFIG ************/

# define DEFAULT_CONFIG_FILE "tools/config/webserv.conf"

# define BLOCK_START '{'
# define BLOCK_END '}'
# define FIELD_DELIMITER ';'
# define COMMENT '#'
# define QUOTES "\"'"
# define S_QUOTE '\''
# define D_QUOTE '"'
# define SPACES " \t\r\n"

enum TokenType
{
	TKN_BLOCK_START,
	TKN_BLOCK_END,
	TKN_FIELD_DELIMITER,
	TKN_DIRECTIVE,
	TKN_VALUE,
};


/************ ERRORS ************/

//--> SocketManager.cpp
# define SOCKET_CREATION_ERROR "Error.\nUnable to create socket's fd\n"
# define SOCKET_BINDING_ERROR "Error.\nUnable to bind socket\n"
# define SOCKET_LISTENING_ERROR "Error.\nUnable to set one socket to a listening state\n"

//--> Blocks_parsing.cpp
# define INVALID_NUMBER_OF_ARGUMENTS_IN "Error.\nInvalid number of arguments in " //*Will be used with the block's name
# define INVALID_VALUE_IN "Error.\nInvalid value in "
# define INVALID_PARAMETER_IN "Error.\nInvalid parameter "

# define INVALID_VALUE "Error.\nInvalid value "
# define INVALID_RETURN_CODE "Error.\nInvalid return code "
# define ERROR_PAGE_OUT_OF_BOUND "Error.\nerror_page's value must be between 300 and 599"
# define PORT_OUT_OF_BOUND "Error.\nPort's value must be between 0 and 65535\n"
# define METHOD_UNKNOWN "Error.\nUnknown method "


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
