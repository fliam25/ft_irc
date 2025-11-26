/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 04:38:02 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/26 13:50:55 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "Server.hpp"
# include "replies.hpp"

CommandHandler::CommandHandler(Server& server) : _server(server) 
{
	_initializeCommands();
}

void	CommandHandler::_initializeCommands()
{
	_commands["PASS"] = &CommandHandler::_Pass;
	_commands["NICK"] = &CommandHandler::_Nick;
	_commands["USER"] = &CommandHandler::_User;
	_commands["JOIN"] = &CommandHandler::_Join;
	_commands["KICK"] = &CommandHandler::_Kick;
	_commands["MODE"] = &CommandHandler::_Mode;
	_commands["TOPIC"] = &CommandHandler::_Topic;
	_commands["PRIVMSG"] = &CommandHandler::_Privmsg;
}

bool	CommandHandler::_checkRegistered(Client& client, const std::string& cmd) const
{
	if (cmd == "PASS" || cmd == "NICK" || cmd == "USER")
		return (1);
	return (client.getRegistered());
}

void	CommandHandler::_uppercase(std::string& str)
{
	if (str.empty())
		return;
	for (size_t i = 0; i < str.length(); ++i)
	{
		str[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));
	}
}

void	CommandHandler::handleRequest(Client *client)
{
	std::string	&buf = client->getBuffer();

	size_t	pos;
	while (((pos = buf.find("\r\n")) != std::string::npos) || ((pos = buf.find("\n")) != std::string::npos))
	{
		std::string	line = buf.substr(0, pos);
		if (pos + 1 < buf.size() && buf[pos] == '\r' && buf[pos + 1] == '\n')
			buf.erase(0, pos + 2);
		else
			buf.erase(0, pos + 1);

		if (line.size() > 512)
		{
			_server.sendToClient(*client, "ERROR :Input line too long");
			_server.disconnectClient(*client);
			return;
		}
	
		_setCommandAndParams(line);
		_runCommand(client);
	}
}

void	CommandHandler::_setCommandAndParams(const std::string& line)
{
	if (line.empty())
		return;
	
	_command.clear();
	_params.clear();

	std::string			tmp;
	std::stringstream	iss(line);
	std::cout << line << std::endl;
	//set command
	iss >> tmp;
	if (!tmp.empty() && tmp[0] == ':')
		iss >> _command;
	else
		_command = tmp;
	_uppercase(_command);

	//set params
	tmp.clear();
	while (iss >> tmp)
	{
		if (!tmp.empty() && tmp[0] == ':')
		{
			std::string	trailing = tmp.substr(1);
			std::string	buf;
			std::getline(iss, buf);
			if (!buf.empty() && buf[0] == ' ')
				buf.erase(0, 1);
			if (!buf.empty())
				trailing += " " + buf;
			_params.push_back(trailing);
			break;
		}
		else
			_params.push_back(tmp);
	}
}

void	CommandHandler::_runCommand(Client *client)
{
	if (_command.empty())
	{
		sendErrorCommand(_server, *client, "*");
		return;
	}
	if (_command == "QUIT")
		_server.setLoop();

	std::map<std::string, CommandFunc>::iterator it = _commands.find(_command);
	if (it != _commands.end())
	{
		if (!_checkRegistered(*client, _command))
			sendErrorNotRegister(_server, *client);
		else
		{
			CommandFunc	func = it->second;
			(this->*func)(*client, _params);
		}
	}
	else
		sendErrorCommand(_server, *client, _command);
}

CommandHandler::~CommandHandler() {}
