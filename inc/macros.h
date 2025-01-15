/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:38:27 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/15 17:19:57 by mbecker          ###   ########.fr       */
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


/************ ERRORS ************/

# define SOCKET_CREATION_ERROR "Error.\nUnable to create socket's fd."
# define SOCKET_BINDING_ERROR "Error.\nUnable to bind socket."
# define SOCKET_LISTENING_ERROR "Error.\nUnable to set one socket to a listening state."



#endif
