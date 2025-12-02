# include "commands.h"
# include "CommandHandler.hpp"

void CommandHandler::_Mode(Client& client, const std::vector<std::string>& params)
{
    if (params.empty())
    {
        sendErrorMoreParams(this->_server, client, "MODE");
        return;
    }
    const std::string& target = params[0];
    Channel* chan = this->_server.FindChannel(target);
    if (!chan)
    {
        sendErrorNoSuchChannel(this->_server, client, target);
        return;
    }
	if (!chan->isInChannel(client))
    {
        sendErrorNotOnChannel(this->_server, client, target);
        return;
    }
    if (params.size() == 1)
    {
        sendChannelModes(this->_server, client, *chan);
        return;
    }
    if (!chan->isOperator(client))
    {
        sendErrorChannelOperatorNeeded(this->_server, client, target);
        return;
    }
    const std::string& modeString = params[1];
    bool adding = true;
    std::string appliedModes = "";
    std::vector<std::string> appliedArgs; 
    size_t argIndex = 2;
    for (size_t i = 0; i < modeString.size(); ++i)
    {
        char c = modeString[i];
        if (c == '+') { adding = true; appliedModes += '+'; continue; }
        if (c == '-') { adding = false; appliedModes += '-'; continue; }
        switch(c)
        {
            case 'i':
                chan->SetInviteOnly(adding);
                appliedModes += 'i';
                continue;

            case 't':
                chan->SetTopicOperator(adding);
                appliedModes += 't';
                continue;

            case 'k':
                if (adding)
                {
                    if (argIndex >= params.size())
                    {
                        sendErrorMoreParams(this->_server, client, "MODE");
                        continue;
                    }
                    chan->SetChannelKey(params[argIndex]);
                    appliedModes += 'k';
                    appliedArgs.push_back(params[argIndex]);
                    argIndex++;
                }
                else
                {
                    chan->SetChannelKey("");
                    appliedModes += 'k';
                }
                continue;
            case 'l':
                if (adding)
                {
                    if (argIndex >= params.size())
                    {
                        sendErrorMoreParams(this->_server, client, "MODE");
                        continue;
                    }
                    int limit = atoi(params[argIndex].c_str());
                    if (limit <= 0) limit = 0;
                    chan->SetUserLimit(limit);
                    appliedModes += 'l';
                    appliedArgs.push_back(params[argIndex]);
                    argIndex++;
                }
                else
                {
                    chan->SetUserLimit(0);
                    appliedModes += 'l';
                }
               	continue;

            case 'o':
                if (argIndex >= params.size())
                {
                    sendErrorMoreParams(this->_server, client, "MODE");
                   continue;
                }
				else
                {
                    Client* targetClient = this->_server.findClient(params[argIndex]);
                    if (!targetClient || !chan->isInChannel(*targetClient))
                    {
                        sendErrorNotOnChannel(this->_server, client, params[argIndex]);
                        continue;
                    }
                    if (adding)
                        chan->AddOperator(*targetClient);
                    else
                        chan->RemoveOperator(*targetClient);

                    appliedModes += 'o';
                    appliedArgs.push_back(params[argIndex]);
                    argIndex++;
                }
                continue;
            default:
            	sendErrorUnknownMode(this->_server, client, c);
                continue;
        }
    }
    if (!appliedModes.empty())
    {
        std::stringstream ss;
        ss << ":" << client.getNick() << "!" << client.getUser() << "@localhost"
           << " MODE " << chan->GetName() << " " << appliedModes;
        for (size_t i = 0; i < appliedArgs.size(); ++i)
            ss << " " << appliedArgs[i];
        ss << "\r\n";
		_server.SendToAllClientInChannel(ss.str(), *chan, client, false);
    }
}
