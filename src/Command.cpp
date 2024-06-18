#include "../inc/Command.hpp"
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
    std::cout << PURPLE << "Command constructor: argCount: " << this->_argCount << " _args: "; //para probar
    for (int i = 0; i < this->_argCount; i++)
    {
        std::cout << "arg[" << i << "]: " << this->_args[i] << ' ';
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

void Command::parseCommand(const std::string &cmd, Server* server, User& user)
{
    if (this->_argCount == 0)
    {
        user.enqueueResponse(err_unknowncommand(*server, user, *this));
        std::cout << user.dequeueResponse(); //ahora mismo es para probar?
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
    else
    {
        user.enqueueResponse(err_unknowncommand(*server, user, *this));
        std::cout << user.dequeueResponse(); //ahora mismo es para probar?
    }
    return ;
}
