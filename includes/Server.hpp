/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 20:57:40 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/29 16:00:25 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "include.h"
# include "Client.hpp"
# include "Channel.hpp"
# include "CommandHandler.hpp"
# include "commands.h"

class Server
{
	public:
			//Constructors
			Server(int port, std::string password);
			~Server();

			//public Functions
			void					run();
			void					sendToClient(Client& client, const std::string& message);
			void					disconnectClient(Client& client);
			bool					findNickname(const std::string& nick) const;		
			Client*					findClient(const std::string& nick) const;
			Channel	* 				FindChannel(std::string name);
			bool					IsChannelNameValid(std::string name);
			bool					addChannel(std::string name, Client& Owner, std::string pass);
			void					SendToAllClientInChannel(const std::string& string, Channel &chan, Client &toignore, bool ignore);
			bool 					isValidChannelKey(const std::string &key);
			void					RemoveClientFromAllChannels(Client &client);

			//Getters
			const std::string&		getPass() const;
			int						getEpoll() const;
			Client*					getClient(int fd);

			//Setters
			void					setEpoll(int fd);

	//Exception
	class SocketException : public std::runtime_error
	{
		public:
				SocketException(const std::string &msg) : std::runtime_error(msg) {}
	};

	private:
			std::string						_password;
			int								_port;
			int								_socket;
			int								_epollFd;
			std::map<int, Client*>			_clients;
			std::map<std::string, Channel*>	_channels;
			CommandHandler					_handleCommand;

			//private functions
			int			_setupSocket(int port);
			void		_welcome(void) const;
			bool		_makeNoblockFd(int fd);
			bool		_addClient(int clientfd);
			bool		_readClientRequest(int fd);
			void		_deleteClient(Client& client);
			std::string _StandardizeChannelName(std::string name);
};

#endif