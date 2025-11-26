/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 03:09:09 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/25 10:14:27 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

//	Constructor

Server::Server(int port, std::string password) : _handleCommand(*this)
{
	_loop = 1;
	_port = port;
	_password = password;
	_epollFd = 0;
	_socket = Server::_setupSocket(port);
}

//	Socket Gestion

int		Server::_setupSocket(int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		throw std::runtime_error("Server: can't create socket");

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(fd);
		throw std::runtime_error("Server: setsockopt failed");
	}

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(fd);
		throw std::runtime_error("Server: can't bind socket");
	}

	if (listen(fd, SOMAXCONN) < 0) 
	{
		close(fd);
		throw std::runtime_error("Server: can't listen socket");
	}
	if (!_makeNoblockFd(fd))
	{
		close(fd);
		throw std::runtime_error("Server: fcntl failed");
	}
	return (fd);
}

bool	Server::_makeNoblockFd(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (0);
	return (1);
}

//	Gestion du Réseau

void	Server::run()
{
	int epollFd = epoll_create(1);
	if (epollFd == -1)
		throw std::runtime_error("Server: epoll_create failed");

	setEpoll(epollFd);
	struct epoll_event ev, events[MAX_EVENTS];
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = _socket;

	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socket, &ev) == -1)
		throw std::runtime_error("Server: epoll_ctl add server socket failed");
	
	_welcome();

	while (_loop)
	{
		int n = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (n == -1)
		{
			if (errno == EINTR)
				continue;
			throw std::runtime_error("Server: epoll_wait failed");
		}
		for (int i = 0; i < n; ++i)
		{
			int	fd = events[i].data.fd;
			if (fd == _socket)
			{
				// New client
				int clientFd = accept(_socket, NULL, NULL);
				if (clientFd == -1) 
				{
					if (errno != EAGAIN && errno != EWOULDBLOCK)
						std::cerr << "accept failed" << std::endl;
					continue;
				}
				if (!_addClient(clientFd))
        			std::cerr << "Failed to add client " << clientFd << std::endl;
			}
			else
			{
				// Reading clients
				if (_readClientRequest(fd))
					_handleCommand.handleRequest(getClient(fd));
			}
		}
	}
}

//	Fonctions Membres

bool	Server::_addClient(int clientfd)
{
	if (_clients.find(clientfd) != _clients.end())
	{
		std::cerr << "Error: client fd " << clientfd << " already exists!" << std::endl;
		close(clientfd);
		return (0);
	}

	if (!_makeNoblockFd(clientfd))
	{
		close(clientfd);
		return (0);
	}

	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = clientfd;

	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientfd, &ev) == -1)
	{
		close(clientfd);
		return (0);
	}

	_clients[clientfd] = new Client(clientfd);
	return (1);
}

bool	Server::_readClientRequest(int fd)
{
	Client	*client = getClient(fd);
	if (!client)
	{
		std::cerr << "Server: unknown client fd " << fd << std::endl;
		close(fd);
		return (0);
	}

	char	buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	ssize_t	n = recv(fd, buffer, BUFFER_SIZE, 0);
	while (n == -1 && errno == EINTR)
		n = recv(fd, buffer, BUFFER_SIZE, 0);

	while (n >= 0)
	{
		if (n == 0)
		{
			_deleteClient(*client);
			return (0);
		}
		else if (n > 0)
			client->appendToBuffer(buffer, n);

		n = recv(fd, buffer, BUFFER_SIZE, 0);
		while (n == -1 && errno == EINTR)
			n = recv(fd, buffer, BUFFER_SIZE, 0);
	}

	if (errno != EAGAIN && errno != EWOULDBLOCK)
	{
		std::cerr << "recv error on fd " << fd << std::endl;
		_deleteClient(*client);
		return (0);
	}

	return (1);
}

void	Server::sendToClient(Client& client, const std::string& message)
{
	int fd = client.getFd();
	if (send(fd, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Error sending to client " << fd << std::endl;
		_deleteClient(client);
	}
}

void	Server::disconnectClient(Client& client)
{
	_deleteClient(client);
}

void Server::_deleteClient(Client& client)
{
	RemoveClientFromAllChannels(client);
	int fd = client.getFd();
	if (fd >= 0)
		epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);

	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
}

bool	Server::findNickname(const std::string& nick) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* c = it->second;
		if (c && c->getNick() == nick)
			return (1);
	}
	return (0);
}

void	Server::_welcome(void) const
{
	std::cout << " ________________________________________________________________________" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************* B O N S O I R ********************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************ It 's IRC BABY !*******************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl;
	std::cout << "|************************************************************************|" << std::endl << std::endl;
}

//	Getters and Setters

//	Getters

const std::string&		Server::getPass() const
{
	return (_password);
}

int			Server::getEpoll() const
{
	return (_epollFd);
}

Client*		Server::getClient(int fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return it->second; 
	return (0);
}

//	Setters

void		Server::setEpoll(int fd)
{
	_epollFd = fd;
}

void		Server::setLoop()
{
	_loop = 0;
}

//	Destructor

Server::~Server()
{
	if (_socket >= 0)
		close(_socket);
	while (!_clients.empty())
	{
		Client* c = _clients.begin()->second;
		_deleteClient(*c); 
	}
	if (_epollFd >= 0)
		close(_epollFd);
	while (!_channels.empty())
	{
		Channel* chan = _channels.begin()->second;
		delete chan;
		_channels.erase(_channels.begin());
	}
}
