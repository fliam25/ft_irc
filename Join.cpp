# include "commands.h"
# include "CommandHandler.hpp"

std::vector<std::string>	SplitVector(std::string string, char del)
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
void	CommandHandler::_Join(Client& client, const std::vector<std::string>& params)
{
	std::vector<std::string> Channels;
	std::vector<std::string> Pass;
	if (params.size() > 2 || params.size() <= 0)
	{
		sendErrorMoreParams(this->_server, client, "JOIN");
		return;
	}
	Channels = SplitVector(params[0], ',');
	if(params.size() == 2)
	{
		Pass = SplitVector(params[1], ',');
		if (Pass.size() > Channels.size())
		{
			sendErrorMoreParams(this->_server, client, "JOIN");
			return;
		}
	}
	for(size_t i = 0; i < Channels.size(); ++i)
	{
		if(this->_server.IsChannelNameValid(Channels[i]))
		{
			Channel *channel_ptr = this->_server.FindChannel(Channels[i]);
			std::string passtouse = "";
			if (Pass.size() > i)
			{
				if(this->_server.isValidChannelKey(Pass[i]))
					passtouse = Pass[i];
				else
				{
					sendErrorBadChannelKey(this->_server, client, Channels[i]);
					continue;
				}
			}
			if(channel_ptr == NULL)
			{
				this->_server.addChannel(Channels[i], client, passtouse);
			}
			else
			{
				if(channel_ptr->IsAllowedToJoin(client, passtouse))
				{
					channel_ptr->AddClient(client);
				}
			}
		}
		else
		{
			sendErrorBadChannelMask(this->_server, client, Channels[i]);
		}
	}
}
