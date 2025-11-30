/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 20:50:22 by mqwa              #+#    #+#             */
/*   Updated: 2025/11/29 16:11:37 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "include.h"
# include "Server.hpp"

volatile sig_atomic_t	g_stop = 1;

void	handle_sigint(int sig)
{
	(void)sig;
	g_stop = 0;
}

bool	setupSigint()
{
	struct sigaction	sa;
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "error: sigaction error" << std::endl;
		return (0);
	}
	return (1);
}

static bool	onlyDigits(char *str)
{
	if (!str || !str[0])
		return (0);
	for (int i = 0; str[i]; i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return (0);
	}
	return (1);
}

static bool	validateArguments(int ac, char **av, int& port, std::string& password)
{
	if (ac != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	if (!onlyDigits(av[1]))
	{
		std::cerr << "Invalid port: must contain only digits" << std::endl;
		return (0);
	}
	port = atoi(av[1]);
	if (port < 1024 || port > 65535)
	{
		std::cerr << "Invalid port: must be between 1024 and 65535" << std::endl;
		return (0);
	}

	password = av[2];
	if (password.empty())
	{
		std::cerr << "Password cannot be empty" << std::endl;
		return (0);
	}
	return (1);
}

int		main(int ac, char **av)
{
	int				port;
	std::string		password;

	if (!setupSigint())
		return (1);
	if (!validateArguments(ac, av, port, password))
		return (1);
	try
	{
		Server	server(port, password);
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
