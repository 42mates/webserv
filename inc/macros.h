/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:38:27 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/17 15:53:23 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

using namespace std;

/************ CONFIG ************/

# define DEFAULT_CONFIG_FILE "tools/config/webserv.conf"


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
# define ERROR_PAGE_OUT_OF_BOUND "Error.\nerror_page's value must be between 300 and 599\n"
# define PORT_OUT_OF_BOUND "Error.\nPort's value must be between 0 and 65535\n"
# define METHOD_UNKNOWN "Error.\nUnknown method "

#endif
