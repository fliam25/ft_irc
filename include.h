/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 20:54:41 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/29 16:02:39 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include <cstring>
# include <cstdlib>
# include <csignal>
# include <cstdio>
# include <cerrno>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <exception>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <map>
# include <vector>
# include <set>

extern volatile sig_atomic_t g_stop;

# define MAX_EVENTS		1024
# define BUFFER_SIZE    1024
# define SERVER_NAME	"irc.localhost"
# define VER			"42_IRC-1.0"
# define DATE           "01-11-2025"
