/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 21:38:41 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/26 11:36:57 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "commands.h"

void	CommandHandler::_User(Client& client, const std::vector<std::string>& params)
{
	if (!client.getHasPass())
	{
		sendGenericError(_server, client, "You must enter PASS before using USER\r\n");
		return;
	}
	if (client.getRegistered())
	{
		sendErrorRegistred(_server, client);
		return;
	}
	if (params.size() != 4 || params[0].empty() || params[3].empty())
	{
		sendErrorMoreParams(_server, client, "USER");
		return;
	}

	std::string	username = params[0];
	std::string	realname = params[3];

	client.setUser(username);
	client.setRealName(realname);

	client.setServerName(params[1]);
	client.setHostName(params[2]);

	if (!client.getRegistered() && !client.getNick().empty())
	{
		client.setRegistered();
		sendAllRpl(_server, client);
	}
}
