/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:00:12 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/29 15:36:22 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "commands.h"

static const std::string	getPrefixe(Client& client)
{
	std::string	prefixe;
	prefixe = ":" + client.getNick() + "!" + client.getUser() + "@localhost";
	return (prefixe);
}

void	CommandHandler::_Quit(Client& client, const std::vector<std::string>& params)
{
	std::string	reason;
	if (!params.empty())
		reason = params[0];
	else
		reason = "Quit";

	std::string	msg;
	msg = getPrefixe(client) + " QUIT :" + reason + "\r\n";

	const std::set<Channel*>& channels = client.GetChannels();
	for (std::set<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		_server.SendToAllClientInChannel(msg, **it, client, true);
	}

	std::string	error;
	error = "ERROR :Closing Link: " + client.getNick() + " (" + reason + ")\r\n";
	_server.sendToClient(client, error);

	_quit = 1;
	_server.disconnectClient(client);
}
