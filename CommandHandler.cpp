/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 04:38:02 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/30 13:18:27 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommandHandler.hpp"
# include "Server.hpp"
# include "replies.hpp"

CommandHandler::CommandHandler(Server& server) : _server(server) 
{
	_initializeCommands();
	_quit = 0;
}

void	CommandHandler::_initializeCommands()
{
	_commands["PASS"] = &CommandHandler::_Pass;
	_commands["NICK"] = &CommandHandler::_Nick;
	_commands["USER"] = &CommandHandler::_User;
	_commands["JOIN"] = &CommandHandler::_Join;
	_commands["KICK"] = &CommandHandler::_Kick;
	_commands["MODE"] = &CommandHandler::_Mode;
	_commands["QUIT"] = &CommandHandler::_Quit;
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

	while (1)
	{
		size_t	posCRLF = buf.find("\r\n");
		size_t	posLF = buf.find("\n");

		bool hasCRLF = posCRLF != std::string::npos;
		bool hasLF   = posLF != std::string::npos;

		if (!hasCRLF && !hasLF)
			break;

		std::string	line;
		if (hasCRLF && (!hasLF || posCRLF < posLF))
		{
			line = buf.substr(0, posCRLF);
			buf.erase(0, posCRLF + 2);
		}
		else
		{
			line = buf.substr(0, posLF);
			buf.erase(0, posLF + 1);
		}

		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;

		if (line.size() > 512)
		{
			_server.sendToClient(*client, "ERROR :Input line too long");
			_server.disconnectClient(*client);
			return;
		}

		_setCommandAndParams(line);
		_runCommand(client);

		if (_quit)
		{
			_quit = 0;
			break;
		}
	}
}

void	CommandHandler::_setCommandAndParams(const std::string& line)
{
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