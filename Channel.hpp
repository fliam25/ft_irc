/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:10:58 by mqwa              #+#    #+#             */
/*   Updated: 2025/10/28 03:35:53 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "include.h"
# include "Client.hpp"
# include "Server.hpp"
# include "repliesChannel.hpp"

class Channel
{
		public:
				Channel(const std::string name, Client &owner, std::string pass, Server &Server);
				~Channel();
				
				void SetTopic(const std::string topic);
				void SetUserLimit(int limit);
				int GetUserLimit();
				const std::string GetName();
				const std::string GetTopic();
				void AddOperator(Client &client);
				void RemoveOperator(Client &client);
				void AddClient(Client &client);
				void InviteClient(Client &client);
				void UnInviteClient(Client &client);
				void DeleteClient(Client &client);
				bool isInvited(Client &client);
				bool isOperator(Client &client);
				bool isInChannel(Client &client);
				bool IsAllowedToJoin(Client &client, std::string password);
				bool IsTopicOperator();
				void SetInviteOnly(bool val);
				bool IsInviteOnly();
				void SetTopicOperator(bool val);
				void SetChannelKey(std::string key);
				const std::string	GetChannelKey();
				const std::set<Client*>&	GetClientsList();
		private:
				std::string			_name;
				std::string			_topic;
				std::set<Client*>	_operators;
				std::string			_password;
				std::set<Client*>	_clients;
				std::set<Client*>	_invites;
				Server&			_server;
				long unsigned int					_limit;
				bool				_inviteMode;
				bool				_topicoperator;
};

#endif