/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqwa <mqwa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 20:50:22 by mqwa              #+#    #+#             */
/*   Updated: 2025/10/31 02:20:57 by mqwa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "include.h"
# include "Server.hpp"

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
