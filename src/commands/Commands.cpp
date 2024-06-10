#include "../../inc/Commands.hpp"

Command::Command(const std::string &msg)
{
    //spliteo msg en args -> _args
    //relleno argsCount
    std::istringstream iss(msg);
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        this->_args.push_back(token);
    }
    this->_argCount = this->_args.size();
    std::cout << "Command constructor: argCount: " << this->_argCount << " this->_args: "; //para probar
    for (int i = 0; i < this->_argCount; i++)
    {
        std::cout << this->_args[i] << ' ';
    }
    std::cout << '\n';

}

Command::~Command()
{
}



std::string &Command::getArg(int i)
{
    return this->_args[i];
}

void Command::parseCommand(const std::string &cmd, Server* server, User* user)
{
    if (cmd == "NICK")
        executeNick(*this, *server, *user);
    else if (cmd == "TOPIC")
        executeTopic(*this, *server, *user);
    else if (cmd == "PRIVMSG")
        executeTopic(*this, *server, *user);
    else if (cmd == "MODE")
        executeTopic(*this, *server, *user);
}
