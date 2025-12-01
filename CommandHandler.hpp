/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:17:53 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/29 13:35:11 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

# include "include.h"

class Server;
class Client;

std::vector<std::string>	SplitVector(std::string string, char del);

class CommandHandler
{
		public:
					CommandHandler(Server &server);
					~CommandHandler();

					void		handleRequest(Client* client);

		private:
					Server&						_server;
					std::string					_command;
					std::vector<std::string>	_params;
					bool						_quit;

					typedef void (CommandHandler::*CommandFunc)(Client&, const std::vector<std::string>&);
					std::map<std::string, CommandFunc>	_commands;

					void		_initializeCommands();
					bool		_checkRegistered(Client& client, const std::string& cmd) const;
					void		_setCommandAndParams(const std::string& line);
					void		_uppercase(std::string& str);
					void		_runCommand(Client *client);

					void		_Pass(Client& client, const std::vector<std::string>& params);
					void		_Nick(Client& client, const std::vector<std::string>& params);
					void		_User(Client& client, const std::vector<std::string>& params);
					void		_Join(Client& client, const std::vector<std::string>& params);
					void		_Topic(Client& client, const std::vector<std::string>& params);
					void		_Kick(Client& client, const std::vector<std::string>& params);
					void 		_Mode(Client& client, const std::vector<std::string>& params);
					void		_Quit(Client& client, const std::vector<std::string>& params);
					void		_Privmsg(Client& client, const std::vector<std::string>& params);
					void		_Invite(Client& client, const std::vector<std::string>& params);
};

#endif