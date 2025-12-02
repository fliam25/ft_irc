# include "commands.h"
# include "CommandHandler.hpp"


void	CommandHandler::_Invite(Client& client, const std::vector<std::string>& params)
{
	if(params.size() == 2)
	{
		Client *Client_ptr = _server.findClient(params[0]);
		if(Client_ptr == NULL)
			sendErrorNoSuchNick(_server, client, params[0]);
		else
		{
			if(!this->_server.IsChannelNameValid(params[1]))
			{
				sendErrorBadChannelMask(_server,client,params[1]);
				return;
			}
			Channel *channel_ptr = this->_server.FindChannel(params[1]);
			if(channel_ptr != NULL)
			{
				if (!channel_ptr->isInChannel(client))
				{
					sendErrorNotOnChannel(this->_server, client, channel_ptr->GetName());
					return;
				}
				if(channel_ptr->isInChannel(*Client_ptr))
				{
					sendErrorTargetArleadyOnChannel(_server, client, *Client_ptr, params[1]);
					return;
				}
				if(channel_ptr->IsInviteOnly())
				{
					if(!channel_ptr->isOperator(client))
					{
						sendErrorChannelOperatorNeeded(_server, client, params[0]);
						return;
					}
					channel_ptr->InviteClient(*Client_ptr);
				}
			}
			sendInvite(_server, client, *Client_ptr, params[1]);
		}
	}
	else
		sendErrorMoreParams(_server, client, "INVITE");
}