#include "../inc/Command.hpp"
#include "../inc/Server.hpp"
#include <algorithm>

Command::Command(const std::string &msg)
{
    //guardp en un tempral el msg para despues poder quitarle los saltos de linea en caso de que los haya
    std::string tmp = msg;
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
    //procedo a dividir el string por espacios para sacar los argumentos del comando
    std::istringstream iss(tmp);
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        this->_args.push_back(token);
    }
    this->_argCount = this->_args.size();
    if (msg.empty())
        this->_args[0] = "";
/*     std::cout << PURPLE << "Command constructor: argCount: " << this->_argCount << " _args: "; //para probar
    for (int i = 0; i < this->_argCount; i++)
    {
        std::cout << "arg[" << i << "]: " << this->_args[i] << ' ';
    }
    std::cout << END << '\n'; // borrar*/

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
        server->sendMessageClient(user.getFd(), user.dequeueResponse());
        return ;
    }
    if (cmd == "NICK")
        executeNick(*this, *server, user);
    else if (cmd == "TOPIC")
        executeTopic(*this, *server, user);
    else if (cmd == "PRIVMSG")
        executePrivmsg(*this, *server, user);
    else if (cmd == "MODE")
        executeMode(*this, *server, user);
    else if (cmd == "JOIN")
        executeJoin(*this, *server, user);
    else if (cmd == "KICK")
        executeKick(*this, *server, user);
    else if (cmd == "test") //borrar debug
        server->getAllChannelsUserIn(user.getFd());
  /*  else if (cmd == "INVITE")
        executeInvite(*this, *server, *user);
    else if (cmd == "USER")
        executeMode(*this, *server, *user);*/
    else
    {
        user.enqueueResponse(errUnknowncommand(*server, user, *this));
        server->sendMessageClient(user.getFd(), user.dequeueResponse());
    }
    return ;
}

// Implementaci�n del nuevo m�todo
std::string Command::getArgsAsString(int startIdx) const {
    if (startIdx < 0 || startIdx >= static_cast<int>(_args.size())) {
        return "";
    }

    std::string result = "";
    for (int i = startIdx; i < static_cast<int>(_args.size()); ++i) {
        result += _args[i];
        if (i < static_cast<int>(_args.size()) - 1) {
            result += " ";
        }
    }

    return result;
}

void Command::sendMessageToChannels(Server& server, User& user, const std::vector<Channel*>& channels, const std::string& reply)
{
  for (size_t i = 0; i < channels.size(); i++)
  {
    Channel* c = channels[i];
    for (std::map<int, User>::iterator it = c->_users.begin(); it != c->_users.end(); ++it)
    {
        if (it->second.getFd() != user.getFd())
        {
            it->second.enqueueResponse(reply);
            server.sendMessageClient(it->first, it->second.dequeueResponse());
        }
    }
  }
} 