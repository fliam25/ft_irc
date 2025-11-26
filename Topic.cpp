# include "commands.h"
# include "CommandHandler.hpp"


void	CommandHandler::_Topic(Client& client, const std::vector<std::string>& params)
{
	if(params.size() > 0 && params.size() <= 2)
	{
		Channel *channel_ptr = this->_server.FindChannel(params[0]);

		if(channel_ptr == NULL)
		{
			sendErrorNoSuchChannel(_server, client, params[0]);
			return;
		}
		if(params.size() == 1)
		{
			sendTopic(_server, client, channel_ptr->GetName(), channel_ptr->GetTopic());
			return;
		}
		if (params.size() == 2)
		{
			if(channel_ptr->IsTopicOperator() && !channel_ptr->isOperator(client))
			{
				sendErrorChannelOperatorNeeded(_server, client, params[0]);
				return;
			}
			channel_ptr->SetTopic(params[1]);
			broadcastTopic(_server , client, *channel_ptr, params[1]);
		}
	}
	else
		sendErrorMoreParams(_server, client, "TOPIC");
}