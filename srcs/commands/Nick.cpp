/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 23:27:40 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/05 02:34:08 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "commands.h"

static bool	isSpecialChar(char c)
{
	return (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' ||
				c == '^' || c == '{' || c == '}' || c == '|');
}

static bool	onlyValidChar(const std::string& nick)
{
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (!isSpecialChar(nick[i]) && !isalnum(nick[i]))
			return (0);
	}
	return (1);
}

static bool	checkNickName(const std::string& nick)
{
	if (nick.size() < 1 || nick.size() > 9)
		return (0);
	if (!onlyValidChar(nick))
		return (0);
	if (!isSpecialChar(nick[0]) && !isalpha(nick[0]))
		return (0);
	if (nick.size() == 1 && !isalpha(nick[0]))
		return (0);
	return (1);
}

void	CommandHandler::_Nick(Client& client, const std::vector<std::string>& params)
{
	if (!client.getHasPass())
	{
		sendGenericError(_server, client, "You must enter PASS before using NICK\r\n");
		return;
	}
	if (params.empty() || params[0].empty())
	{
		sendErrorNoNick(_server, client);
		return;
	}
	if (!checkNickName(params[0]))
	{
		sendErrorUnvalidNick(_server, client, params[0]);
		return;
	}
	if (params[0] == client.getNick())
		return;
	if (_server.findNickname(params[0]))
	{
		sendErrorNickUse(_server, client, params[0]);
		return;
	}

	client.setNick(params[0]);

	if (!client.getRegistered() && !client.getUser().empty())
	{
		client.setRegistered();
		sendAllRpl(_server, client);
	}
}
