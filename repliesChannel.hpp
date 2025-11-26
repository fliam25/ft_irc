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

#ifndef REPLIES_CHANNEL_HPP
# define REPLIES_CHANNEL_HPP

# include "include.h"

class Client;
class Server;

void	broadcastTopic(Server& server, Client& setter, Channel& chan, const std::string& topic);
void    sendTopic(Server& server, Client& client, const std::string channel, const std::string topic);
void    sendErrorNoSuchNick(Server& server, Client& client);
void    sendErrorNoSuchChannel(Server& server, Client& client, const std::string channel);
void    sendErrorTargetNotOnChannel(Server& server, Client& client, Client& target, const std::string channel);
void    sendErrorNotOnChannel(Server& server, Client& client, const std::string channel);
void    sendErrorChannelIsFull(Server& server, Client& client, const std::string channel);
void    sendErrorInviteOnly(Server& server, Client& client, const std::string channel);
void    sendErrorBadChannelKey(Server& server, Client& client, const std::string channel);
void    sendErrorBadChannelMask(Server& server, Client& client, const std::string channel);
void    sendErrorChannelOperatorNeeded(Server& server, Client& client, const std::string channel);
void	sendClientList(Server& server, Client& client, Channel &channel);
void    sendEndOfNameList(Server& server, Client& client, const std::string channel);
void 	sendChannelModes(Server &server, Client &client, Channel &chan);
void sendErrorUnknownMode(Server& server, Client& client, char mode);

#endif