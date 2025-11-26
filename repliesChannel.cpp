# include "replies.hpp"
# include "Server.hpp"
# include "Client.hpp"

//TOPIC
void broadcastTopic(Server& server, Client& setter, Channel& chan, const std::string& topic)
{
    std::stringstream ss;
    ss << ":" << setter.getNick() << "!" << setter.getUser() << "@localhost" << " TOPIC " << chan.GetName() << " :" << topic << "\r\n";

    server.SendToAllClientInChannel(ss.str(), chan, setter , false);
}


void sendTopic(Server& server, Client& client, const std::string& channel, const std::string& topic)
{
    std::string nick = client.getNick();
    if (nick.empty())
        nick = "*";

    std::stringstream ss;
    ss << ":" << SERVER_NAME << " 332 "
       << nick << " " << channel << " :" << topic << "\r\n";
    ss << ":" << SERVER_NAME << " 333 "
       << nick << " " << channel << " "
       << client.getNick() << " " << time(NULL) << "\r\n";

    server.sendToClient(client, ss.str());
}

void    sendTopic(Server& server, Client& client, const std::string channel, const std::string topic)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 332 " << nick << " " << channel << " :" << topic <<"\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorNoSuchNick(Server& server, Client& client)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 401 " << nick << " :No such nick\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorNoSuchChannel(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 403 " << nick << " " << channel << " :No such channel\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorTargetNotOnChannel(Server& server, Client& client, Client& target, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::string targetnick = target.getNick();
	if (targetnick.empty())
		targetnick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 441 " << nick << " " << targetnick << " " << channel << " :They aren't on that channel\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorNotOnChannel(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 442 " << nick << " " << channel << " :You're not on that channel\r\n";
	server.sendToClient(client, ss.str());
}


void    sendErrorChannelIsFull(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 471 " << nick << " " << channel << " :Cannot join channel (+l)\r\n";
	server.sendToClient(client, ss.str());
}

void sendErrorUnknownMode(Server& server, Client& client, char mode)
{
    std::string nick = client.getNick();
    if (nick.empty())
        nick = "*";

    std::stringstream ss;
    ss << ":" << SERVER_NAME
       << " 472 " << nick
       << " " << mode
       << " :is unknown mode char to me\r\n";

    server.sendToClient(client, ss.str());
}

void    sendErrorInviteOnly(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 473 " << nick << " " << channel << " :Cannot join channel (+i)\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorBadChannelKey(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 475 " << nick << " " << channel << " :Cannot join channel (+k)\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorBadChannelMask(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 476 " << nick << " " << channel << " :Bad Channel Mask\r\n";
	server.sendToClient(client, ss.str());
}

void    sendErrorChannelOperatorNeeded(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 482 " << nick << " " << channel << " :Operator privilege needed\r\n";
	server.sendToClient(client, ss.str());
}

void	sendClientList(Server& server, Client& client, Channel &channel)
{
	int j = 0;
	std::string nick = client.getNick();
	std::string list;
	if (nick.empty())
		nick = "*";
	for (std::set<Client*>::iterator it = channel.GetClientsList().begin(); it != channel.GetClientsList().end(); ++it)
	{
		if(j != 0)
			list += " ";
		if(channel.isOperator(*(*it)))
			list += "@";
		list += (*it)->getNick();
		j++;
		if(j == 5 || it == --channel.GetClientsList().end())
		{
			std::stringstream	ss;
			ss << ":" << SERVER_NAME << " 353 " << nick << " = " << channel.GetName() << " " << list << "\r\n";
			server.sendToClient(client, ss.str());
			j = 0;
			list.clear();
		}
	}
	std::stringstream	end;
	end << ":" << SERVER_NAME << " 366 " << nick << " " << channel.GetName() << " :End of names list\r\n";
	server.sendToClient(client, end.str());
	return;
}

void    sendEndOfNameList(Server& server, Client& client, const std::string channel)
{
	std::string nick = client.getNick();
	if (nick.empty())
		nick = "*";
	std::stringstream	ss;
	ss << ":" << SERVER_NAME << " 366 " << nick << " " << channel << " :End of names list\r\n";
	server.sendToClient(client, ss.str());
}

std::string toString(int n)
{
    std::stringstream ss;
    ss << n;
    return ss.str();
}

void sendChannelModes(Server &server, Client &client, Channel &chan)
{
    std::string modes = "+";
    std::string params = "";

    if (chan.IsInviteOnly())
        modes += "i";
    if (chan.IsTopicOperator())
        modes += "t";
    if (!chan.GetChannelKey().empty())
    {
        modes += "k";
        params += " " + chan.GetChannelKey();
    }
    if (chan.GetUserLimit() > 0)
    {
        modes += "l";
        params += " " + toString(chan.GetUserLimit());
    }
    std::stringstream ss;
    ss << ":" << SERVER_NAME
       << " 324 " << client.getNick()
       << " " << chan.GetName()
       << " " << modes;
    if (!params.empty())
        ss << params;
    ss << "\r\n"; 
    server.sendToClient(client, ss.str());
}
