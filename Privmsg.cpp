# include "commands.h"
# include "CommandHandler.hpp"

void	CommandHandler::_Privmsg(Client& client, const std::vector<std::string>& params)
{
	std::vector<std::string> Targets;
	if (params.size() != 2)
	{
		sendErrorMoreParams(this->_server, client, "JOIN");
		return;
	}
	Targets = SplitVector(params[0], ',');

	for(size_t i = 0; i < Targets.size(); ++i)
	{

		if(!Targets[i].empty() && (Targets[i][0] == '#' || Targets[i][0] == '&'))
		{
			if(this->_server.IsChannelNameValid(Targets[i]))
			{
				Channel *channel_ptr = this->_server.FindChannel(Targets[i]);
				if(channel_ptr == NULL)
				{
					sendErrorNoSuchChannel(this->_server, client, Targets[i]);
				}
				else
				{
					std::stringstream ss;
					ss << ":" << client.getNick() << "!" << client.getUser() << "@localhost"
					<< " PRIVMSG " << channel_ptr->GetName() << " :" << params[1] <<"\r\n";
					ss << "\r\n";
					_server.SendToAllClientInChannel(ss.str(), *channel_ptr, client, true);
				}
			}
			else
			{
				sendErrorBadChannelMask(this->_server, client, Targets[i]);
			}
		}
		else
		{
			Client *Client_ptr = _server.findClient(Targets[i]);
			if(Client_ptr == NULL)
				sendErrorNoSuchNick(_server, client);
			else
			{
					std::stringstream ss;
					ss << ":" << client.getNick() << "!" << client.getUser() << "@localhost"
					<< " PRIVMSG " << Client_ptr->getNick() << " :" << params[1] <<"\r\n";
					ss << "\r\n";
					_server.sendToClient(*Client_ptr, ss.str());
			}
		}

	}
}
