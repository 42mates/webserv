/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:38:27 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/14 16:18:44 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

/************ CONFIG ************/

# define DEFAULT_CONFIG_FILE "tools/config/webserv.conf"


/************ ERRORS ************/

# define SOCKET_CREATION_ERROR "Error.\nUnable to create socket's fd."
# define SOCKET_BINDING_ERROR "Error.\nUnable to bind socket."
# define SOCKET_LISTENING_ERROR "Error.\nUnable to set one socket to a listening state."

#endif
