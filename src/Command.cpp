#include "../inc/Command.hpp"

Command::Command(const std::string &msg)
{
    //spliteo msg en args -> _args
    //relleno argsCount

    //que pasa aqui con los saltos de linea???
    std::istringstream iss(msg);
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        this->_args.push_back(token);
    }
    this->_argCount = this->_args.size();
    std::cout << PURPLE << "Command constructor: argCount: " << this->_argCount << " this->_args: "; //para probar
    for (int i = 0; i < this->_argCount; i++)
    {
        std::cout << this->_args[i] << ' ';
    }
    std::cout << END << '\n';

}

Command::~Command()
{
}



std::string& Command::getArg(int i)
{
    return this->_args[i];
}

Code Command::parseCommand(const std::string &cmd, Server* server, User* user)
{
    std::cout << "desde pasrseCommand: " << cmd << "hello" << std::endl;
    if (this->_argCount == 0)
        err_unknowncommand(*server, *user);
    if (cmd == "NICK" || cmd == "NICK\n")
        executeNick(*this, *server, *user);
    else if (cmd == "TOPIC")
        executeTopic(*this, *server, *user);
    else if (cmd == "PRIVMSG")
        executePrivmsg(*this, *server, *user);
    else if (cmd == "MODE")
        executeMode(*this, *server, *user);
    return ERR_UNKNOWNCOMMAND;
}
