# include "commands.h"
# include "CommandHandler.hpp"

std::vector<std::string>	SplitVectorKick(std::string string, char del)
{
	std::vector<std::string> result;
	std::stringstream ss(string);
    std::string line;

	while(getline(ss, line, del))
	{
		result.push_back(line);
	}
	return result;

}

void	KickClientFromChannel(Server &server, Client &ToKick, Client& Kicker, Channel &chan, std::string reason)
{
	if(chan.isInChannel(Kicker))
	{
		if(chan.isOperator(Kicker))
		{
			//std::cout << Kicker.getNick() << " is operator" << std::endl;
			if(chan.isInChannel(ToKick)) 
			{
				std::stringstream	ss;
				ss << ":" << Kicker.getNick() << "!" <<  Kicker.getUser() << "@localhost KICK " << chan.GetName() << " " << ToKick.getNick() << " :" << reason <<"\r\n";
				server.SendToAllClientInChannel(ss.str(), chan, Kicker, 0);
				if(chan.isOperator(ToKick))
					chan.RemoveOperator(ToKick);
				chan.DeleteClient(ToKick);
				ToKick.DeleteChannel(chan);
			}
			else
				sendErrorTargetNotOnChannel(server, Kicker, ToKick, chan.GetName());
		}
		else
			sendErrorChannelOperatorNeeded(server, Kicker, chan.GetName());
	}
	else
		sendErrorNotOnChannel(server, Kicker, chan.GetName());
		
}

void	CommandHandler::_Kick(Client& client, const std::vector<std::string>& params)
{
	std::vector<std::string> Channels;
	std::vector<std::string> Users;
	if (params.size() < 2 || params.size() > 3)
	{
		sendErrorMoreParams(this->_server, client, "KICK");
		return;
	}
	Channels = SplitVectorKick(params[0], ',');
	Users = SplitVectorKick(params[1], ',');
	std::string reason = "";
	if(params.size() == 3)
		reason = params[2];
	if(Channels.size() == Users.size() || Channels.size() == 1)
	{
		for(size_t i = 0; i < Channels.size(); ++i)
		{
			if(this->_server.IsChannelNameValid(Channels[i]))
			{
				Channel *channel_ptr = this->_server.FindChannel(Channels[i]);
				if(channel_ptr == NULL)
					sendErrorBadChannelMask(_server, client, Channels[i]);
				else
				{
					if(Channels.size() == 1)
					{
						for(size_t j = 0; j < Users.size(); ++j)
						{
							Client *Client_ptr = _server.findClient(Users[j]);
							if(Client_ptr == NULL)
								sendErrorNoSuchNick(_server, client, Users[j]);
							else
								KickClientFromChannel(_server, *Client_ptr, client, *channel_ptr, reason);
						}
					}
					else
					{
							Client *Client_ptr = _server.findClient(Users[i]);
							if(Client_ptr == NULL)
								sendErrorNoSuchNick(_server, client, Users[i]);
							else
								KickClientFromChannel(_server, *Client_ptr, client, *channel_ptr, reason);
					}
				}
			}
			else
			{
				sendErrorBadChannelMask(this->_server, client, Channels[i]);
			}
		}
	}
	else
	{
		sendErrorMoreParams(this->_server, client, "KICK");
	}
}