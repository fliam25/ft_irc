/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:32:17 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/05 02:35:50 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "commands.h"

void	CommandHandler::_Pass(Client& client, const std::vector<std::string>& params)
{
	if (client.getRegistered())
	{
		sendErrorRegistred(_server, client);
		return;
	}
	if (client.getHasPass())
	{
		sendGenericError(_server, client, "Password already sent\r\n");
		return;
	}
	if (params.size() != 1)
	{
		sendErrorMoreParams(_server, client, "PASS");
		return;
	}
	const std::string&	password = params[0];
	if (password == _server.getPass())
		client.setHasPass();
	else
		sendErrorPass(_server, client);
}
