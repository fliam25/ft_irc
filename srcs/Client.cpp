/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 22:24:42 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/25 12:36:28 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"

Client::Client(int fd)
{
	_fd = fd;
	_isRegistered = 0;
	_hasPass = 0;
}

//	Public Functions

void	Client::appendToBuffer(const char *src, size_t len)
{
	_bufferClient.append(src, len);
}

void	Client::appendToBuffer(const std::string &src)
{
	_bufferClient.append(src);
}

void	Client::clearBuffer()
{
	_bufferClient.clear();
}

//	Getters

bool	Client::getRegistered() const
{
	return (_isRegistered);
}

bool	Client::getHasPass() const
{
	return (_hasPass);
}

const std::string&	Client::getNick() const
{
	return (_nickName);
}

const std::string&	Client::getUser() const
{
	return (_userName);
}

std::string&	Client::getBuffer()
{
	return (_bufferClient);
}


int		Client::getFd() const
{
	return (_fd);
}

//	Setters

void	Client::setRegistered()
{
	_isRegistered = 1;
}

void	Client::setHasPass()
{
	_hasPass = 1;
}

void	Client::setNick(const std::string& nick)
{
	_nickName = nick;
}

void	Client::setUser(const std::string& nick)
{
	_userName = nick;
}

void	Client::setRealName(const std::string& name)
{
	_realName = name;
}

void	Client::setHostName(const std::string& name)
{
	_hostName = name;
}

void	Client::setServerName(const std::string& name)
{
	_serverName = name;
}

Client::~Client()
{
	_channels.clear();
	if (_fd >= 0)
	{
		close (_fd);
		_fd = -1;
	}
}

void Client::JoinChannel(Channel &channel)
{
	_channels.insert(&channel);
}

void Client::DeleteChannel(Channel &channel)
{
	_channels.erase(&channel);
}

const std::set<Channel*>& Client::GetChannels() const {
    return _channels;
}