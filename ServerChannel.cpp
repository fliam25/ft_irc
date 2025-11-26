#include "Server.hpp"


bool	Server::addChannel(std::string name, Client &Owner, std::string pass)
{
	std::string normalizedname = _StandardizeChannelName(name);
	if (_channels.find(normalizedname) != _channels.end())
	{
		std::cerr << "Error: channels" << normalizedname << " already exists!" << std::endl;
		return (0);
	}
	_channels[normalizedname] = new Channel(normalizedname, Owner, pass, *this);
	return (1);
}


bool	Server::IsChannelNameValid(std::string name)
{
	if (name.empty())
		return false;
	if(name[0] != '&' && name[0] != '#')
		return false;
	if(name.size() < 2 || name.size() > 50)
		return false;
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == ' ' || name[i] == ',' || name[i] == '\x07')
            return false;

        if (static_cast<unsigned char>(name[i]) < 0x20 || name[i] == 0x7F)
            return false;
    }
	return true;
}


std::string	Server::_StandardizeChannelName(std::string name)
{
	std::string result = name;
	for(size_t i = 0; i < result.size(); ++i)
	{
		if((result[i] >= 'a' && result[i] <= 'z') || result[i] == '{' || result[i] == '}' || result[i] == '|')
			result[i] = result[i] - 32;
	}
	return result;
}

Channel	*Server::FindChannel(std::string name)
{
	std::string normalizedname = _StandardizeChannelName(name);

	std::map<std::string, Channel*>::iterator it = _channels.find(normalizedname);
	if (it != _channels.end())
	{
		return it->second;
	}
	return NULL;
}

Client	*Server::findClient(const std::string& nick) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* c = it->second;
		if (c && c->getNick() == nick)
			return (c);
	}
	return (NULL);
}

void Server::SendToAllClientInChannel(const std::string& string, Channel &chan, Client &toignore, bool ignore)
{
	for (std::set<Client*>::const_iterator it = chan.GetClientsList().begin(); it != chan.GetClientsList().end(); ++it)
	{
		if(*it != &toignore || ignore == false)
		{
			sendToClient(**it, string);
		}
	}
	return;
}

bool Server::isValidChannelKey(const std::string &key)
{
    if (key.empty() || key.length() > 50)
        return false;

    for (size_t i = 0; i < key.length(); ++i)
    {
        unsigned char c = key[i];
        if (c == ' ' || c == ',')
            return false;
        if (c < 33 || c == 127)
            return false;
    }

    return true;
}

void Server::RemoveClientFromAllChannels(Client &client)
{
    std::set<Channel*> channels = client.GetChannels();
    for (std::set<Channel*>::iterator it = channels.begin();
         it != channels.end();
         ++it)
    {
        Channel *chan = *it;
        chan->DeleteClient(client);
		if(chan->isOperator(client))
			chan->RemoveOperator(client);
		if(chan->isInvited(client))
			chan->UnInviteClient(client);
        client.DeleteChannel(*chan);
    }
}