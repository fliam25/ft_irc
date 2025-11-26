/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 05:33:08 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/05 04:35:17 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "include.h"

class Client;
class Server;

//  Errors
void	sendGenericError(Server& server, Client& client, const std::string& msg);
void	sendErrorMoreParams(Server& server, Client& client, const std::string& cmd);
void	sendErrorCommand(Server& server, Client& client, const std::string& cmd);
void	sendErrorUnvalidNick(Server& server, Client& client, const std::string& nick);
void	sendErrorNickUse(Server& server, Client& client, const std::string& nick);
void	sendErrorRegistred(Server& server, Client& client);
void	sendErrorNotRegister(Server& server, Client& client);
void	sendErrorPass(Server& server, Client& client);
void	sendErrorNoNick(Server& server, Client& client);

//  RPL
void	sendAllRpl(Server& server, Client& client);
void	sendWelcome(Server& server, Client& client);
void	sendYourHost(Server& server, Client& client);
void	sendCreated(Server& server, Client& client);
void	sendMyInfo(Server& server, Client& client);

#endif