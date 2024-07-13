#include "../inc/Command.hpp"
#include "../inc/Server.hpp"
#include <algorithm>

Command::Command(const std::string &msg)
{
	//guardp en un tempral el msg para despues poder quitarle los saltos de linea en caso de que los haya
	if (msg.empty())
	{
		this->_args.push_back("");
		this->_argCount = 0;
	}
	std::string tmp = msg;
	tmp.erase(std::remove(tmp.begin(), tmp.end(), '\r'), tmp.end());
	tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
/* 	tmp.erase(std::remove(tmp.begin(), tmp.end(), '\"'), tmp.end());
	tmp.erase(std::remove(tmp.begin(), tmp.end(), '\''), tmp.end()); */
	//procedo a dividir el string por espacios para sacar los argumentos del comando
	std::istringstream iss(tmp);
	std::string token;

	while (std::getline(iss, token, ' '))
		this->_args.push_back(token);
	this->_argCount = this->_args.size();
	
/*   std::cout << BLUE << "Command constructor: argCount: " << this->_argCount << " _args: "; //para probar
	for (int i = 0; i < this->_argCount; i++)
	{
			std::cout << "arg[" << i << "]: " << this->_args[i] << ' ';
	}
	std::cout << END << '\n'; // borrar */
}

Command::~Command()
{
}

std::string& Command::getArg(int i)
{
    return this->_args[i];
}

void Command::parseCommand(const std::string &cmd, Server* server, User& user)
{
	if (this->_argCount == 0)
	{
		user.enqueueResponse(errUnknowncommand(*server, user, *this));
		return ;
	}
	else if (cmd == "NICK")
		executeNick(*this, *server, user);
	else if (cmd == "TOPIC" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
		executeTopic(*this, *server, user);
	else if (cmd == "PRIVMSG" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
		executePrivmsg(*this, *server, user);
	else if (cmd == "MODE" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
		executeMode(*this, *server, user);
	else if (cmd == "JOIN" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
		executeJoin(*this, *server, user);
	else if (cmd == "KICK" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
		executeKick(*this, *server, user);
	else if (cmd == "INVITE" && server->_usersServerByFd[user.getFd()]->getConfigOK() == true)
			executeInvite(*this, *server, user);
	else if (cmd == "USER" && server->_usersServerByFd[user.getFd()]->getCommandUSER() == false)
			executeUser(*this, *server, user);
	else
		user.enqueueResponse(errUnknowncommand(*server, user, *this));
	return ;
}

// Metodo para sacar comandos desde startIdx hasta el final en un solo string
std::string Command::getArgsAsString(int startIdx) const
{
	if (startIdx < 0 || startIdx >= static_cast<int>(_args.size()))
		return "";
	std::string result = "";
	for (int i = startIdx; i < static_cast<int>(_args.size()); ++i)
	{
		result += _args[i];
		if (i < static_cast<int>(_args.size()) - 1)
			result += " ";
	}
	return result;
}

void Command::sendMessageToChannels(User& user, const std::vector<Channel*>& channels, const std::string& reply)
{
  for (size_t i = 0; i < channels.size(); i++)
  {
    Channel* c = channels[i];
    for (std::map<int, User*>::iterator it = c->_users.begin(); it != c->_users.end(); ++it)
    {
			if (it->second->getFd() != user.getFd())
				it->second->enqueueResponse(reply);
    }
  }
}

bool Command::isAllowedSymbol(char c)
{
    if ((c == '[') || (c == ']')
        || (c == '{') || (c == '}') || (c == '\\') || (c == '|') || (c == '_')
        || (c == '-') || (c == '`'))
        return true;
    return false;
}