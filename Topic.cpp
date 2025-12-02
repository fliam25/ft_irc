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
		if (!channel_ptr->isInChannel(client))
    	{
        	sendErrorNotOnChannel(this->_server, client, channel_ptr->GetName());
        	return;
    	}
		if(params.size() == 1)
		{
			if((channel_ptr->GetTopic()).empty())
				sendNoTopic(_server, client, channel_ptr->GetName());
			else
				sendTopic(_server, client, *channel_ptr, channel_ptr->GetTopic());
			return;
		}
		if (params.size() == 2)
		{
			if(channel_ptr->IsTopicOperator() && !channel_ptr->isOperator(client))
			{
				sendErrorChannelOperatorNeeded(_server, client, params[0]);
				return;
			}
			if(params[1].empty())
				channel_ptr->ClearTopic();
			else
				channel_ptr->SetTopic(params[1], client.getNick());
			broadcastTopic(_server , client, *channel_ptr, params[1]);
		}
	}
	else
		sendErrorMoreParams(_server, client, "TOPIC");
}