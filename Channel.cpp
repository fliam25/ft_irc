#include "Channel.hpp"

Channel::Channel(const std::string name, Client &Owner, std::string pass, Server &Server) : _name(name), _server(Server)
{
	if(_server.isValidChannelKey(pass))
	{
		_password = pass;
	}
	else
		_password = "";
	_inviteMode = false;
	_topicoperator = false;
	_limit = 0;
	std::cout << "Created new channel " << _name << " with owner " << Owner.getNick() << " and password " << _password << std::endl;
	Owner.JoinChannel(*this);
	this->AddOperator(Owner);
	this->AddClient(Owner);

}
Channel::~Channel()
{
	// this->_clients.clear();
	// this->_operators.clear();
	// this->_invites.clear();
}

const std::string Channel::GetName()
{
	return this->_name;
}

const std::string Channel::GetTopic()
{
	return this->_topic;
}

const std::set<Client*>&	Channel::GetClientsList()
{
	return this->_clients;
}

void Channel::SetTopic(const std::string topic)
{
	this->_topic = topic;
	
}

void Channel::ClearTopic()
{
	if(!this->_topic.empty())
		this->_topic.clear();
}

void Channel::SetTopicOperator(bool val)
{
	this->_topicoperator = val;
}

void Channel::SetInviteOnly(bool val)
{
	this->_topicoperator = val;
}

void Channel::SetChannelKey(std::string key)
{
		this->_password = key;
}
const std::string Channel::GetChannelKey()
{
	return this->_password;
}

void Channel::SetUserLimit(int limit)
{
	this->_limit = limit;
}

int Channel::GetUserLimit()
{
	return this->_limit;
}

void Channel::AddOperator(Client &client)
{
	//std::cout << "NEW OPERATOR" << std::endl;
	std::set<Client*>::iterator it = _operators.find(&client);
	if (it != _operators.end())
	{
		return;
	}
	this->_operators.insert(&client);
}

void Channel::RemoveOperator(Client &client)
{
	std::set<Client*>::iterator it = _operators.find(&client);
	if (it != _operators.end())
	{
			this->_operators.erase(&client);
	}

}

bool Channel::isInChannel(Client &client)
{
	std::set<Client*>::iterator it = _clients.find(&client);
	if (it != _clients.end())
	{
		return true;
	}
	return false;
}

void Channel::AddClient(Client &client)
{
	this->_clients.insert(&client);
	std::stringstream	ss;
	ss << ":" << client.getNick() << " JOIN :" << this->GetName() << "\r\n";
	if(isInvited(client))
		UnInviteClient(client);
	this->_server.SendToAllClientInChannel(ss.str(), *this, client, false);
	sendClientList(this->_server, client, *this);
	if(!_topic.empty())
		sendTopic(_server, client, _name, _topic);
	else
		sendNoTopic(_server, client, this->GetName());
}

void Channel::InviteClient(Client &client)
{
	this->_invites.insert(&client);
}

void Channel::UnInviteClient(Client &client)
{
	this->_invites.erase(&client);
}

bool Channel::isOperator(Client &client)
{
	std::set<Client*>::iterator it = _operators.find(&client);
	if (it != _operators.end())
	{
		return true;
	}
	return false;
}

bool Channel::IsTopicOperator()
{
	if (this->_topicoperator)
		return true;
	return false;
}


bool Channel::IsInviteOnly()
{
	if(_inviteMode == true)
		return true;
	return false;
}

bool Channel::isInvited(Client &client)
{
	std::set<Client*>::iterator it = _invites.find(&client);
	if (it != _invites.end())
	{
		return true;
	}
	return false;
}

void Channel::DeleteClient(Client &client)
{
	std::set<Client*>::iterator it = _clients.find(&client);
	if (it != _clients.end())
	{		
		_clients.erase(it);
	}
}

bool Channel::IsAllowedToJoin(Client &client, std::string password)
{
	if(this->_clients.find(&client) != this->_clients.end())
	{
		//client arleady in channel
		return false;
	}
	if(this->_inviteMode == true && this->isInvited(client) == false)
	{
		sendErrorInviteOnly(this->_server,  client, this->_name);
		return false;
	}
	if(this->_clients.size() != 0 && this->_limit > 0 && this->_clients.size() >= this->_limit)
	{
		sendErrorChannelIsFull(this->_server, client, this->_name);
		return false;
	}
	if(this->_password != "" && this->_password != password)
	{
		sendErrorBadChannelKey(this->_server, client , this->_name);
		return false;
	}
	return true;
}