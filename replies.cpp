/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:09:48 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/05 04:37:50 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "replies.hpp"
# include "Server.hpp"
# include "Client.hpp"

//	RPL

void    sendAllRpl(Server& server, Client& client)
{
	sendWelcome(server, client);
	sendYourHost(server, client);
	sendCreated(server, client);
	sendMyInfo(server, client);
}

void    sendWelcome(Server& server, Client& client)
{
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 001 " << client.getNick() << " :Welcome to the Internet Relay Network "
		<< client.getNick() << "!" << client.getUser() << "@localhost\r\n";
	server.sendToClient(client, ss.str());
}

void    sendYourHost(Server& server, Client& client)
{
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 002 " << client.getNick() << " :Your host is " << SERVER_NAME
		<< ", running version " << VER << "\r\n";
	server.sendToClient(client, ss.str());
}

void    sendCreated(Server& server, Client& client)
{
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 003 " << client.getNick() << " :This server was created "
		<< DATE << "\r\n";
	server.sendToClient(client, ss.str());
}

void sendMyInfo(Server& server, Client& client)
{
	std::stringstream ss;
	ss << ":" << SERVER_NAME << " 004 " << client.getNick() << " "
		<< SERVER_NAME << " " << VER << " o itklo\r\n";
	server.sendToClient(client, ss.str());
}

//	ERRORS

void	sendErrorCommand(Server& server, Client& client, const std::string& cmd)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 421 " << nick << " " << cmd << " :Unknown command\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorNoNick(Server& server, Client& client)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 431 " << nick << " :No nickname given\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorUnvalidNick(Server& server, Client& client, const std::string& target)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 432 " << nick << " " << target << " :Erroneous nickname\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorNickUse(Server& server, Client& client, const std::string& target)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 433 " << nick << " " << target << " :Nickname is already in use\r\n";
	server.sendToClient(client, ss.str());
}

void	sendErrorNotRegister(Server& server, Client& client)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 451 " << nick << " :You have not registered\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorMoreParams(Server& server, Client& client, const std::string& cmd)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 461 " << nick << " " << cmd << " :Not enough parameters\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorRegistred(Server& server, Client& client)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 462 " << nick << " :Unauthorized command (already registered)\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorPass(Server& server, Client& client)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 464 " << nick << " :Password incorrect\r\n";
	server.sendToClient(client, ss.str());
}

void	sendGenericError(Server& server, Client& client, const std::string& msg)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " NOTICE " << nick << " :" << msg;
	server.sendToClient(client, ss.str());
}

