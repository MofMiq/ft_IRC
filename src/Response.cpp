#include "../inc/Response.hpp"
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"


std::string to_string(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

void redirectMessage(Server &server, User &user, Code reply, Command& cmd) //esta funcion no tiene que sacar nada por pantalla per se, pero lo voy a hacer para ir viendo que el parseo funciona
{
    if (reply == ERR_NONICKNAMEGIVEN)
        std::cout << err_nonicknamegiven(server, user) << std::endl;
    else if (reply == ERR_ERRONEUSNICKNAME)
        std::cout << err_erroneusnickname(server, user) << std::endl;
    else if (reply == ERR_NICKNAMEINUSE)
        std::cout << err_nicknameinuse(server, user) << std::endl;
    else if (reply == ERR_UNKNOWNCOMMAND)
        std::cout << err_unknowncommand(server, user) << std::endl;
    else if (reply == RLP_NICKOK)
        std::cout << rlp_nickok(server, user, cmd) << std::endl;
}

std::string createMessage(Server &server, User &user, Code Code)
{
    std::string from = server.getServerName();
    std::string to = user.getNickname();
    std::string msg = ":" + from + " " + to_string(Code) + " " + to + " : ";
    return msg;
}

std::string createReply(Server &server, User &user, Code Code, Command& cmd)
{
    (void)server;
    (void)Code;
    std::string from = user.getOldNick();
    std::string to = user.getNickname();
    std::string msg = ": " + from + "!" + user.getUsername() + "@" + cmd.getArg(0) + to;
    return msg;
}

std::string err_unknowncommand(Server &server, User &user)
{
    return (createMessage(server, user, ERR_UNKNOWNCOMMAND) + "Unknown command" + END);
}

std::string err_nonicknamegiven(Server &server, User &user)
{
    return (createMessage(server, user, ERR_NONICKNAMEGIVEN) + "No nickname given" + END);
}

std::string err_erroneusnickname(Server& server, User& user)
{
    return (createMessage(server, user, ERR_ERRONEUSNICKNAME) + "Erroneus nickname" + END);
}

std::string err_nicknameinuse(Server& server, User& user)
{
    return (createMessage(server, user, ERR_NICKNAMEINUSE) + "Nickname collision" + END);
}

std::string rlp_nickok(Server &server, User &user, Command& cmd)
{
    return (createReply(server, user, RLP_NICKOK, cmd) + "Nick succesfullty changed" + END);
}