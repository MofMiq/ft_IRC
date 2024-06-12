#include "../inc/Response.hpp"
#include "../inc/Server.hpp"


std::string to_string(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

void redirectMessage(Server &server, User &user, Code reply)
{
    if (reply == ERR_NONICKNAMEGIVEN)
        err_nonicknamegiven(server, user);
    else if (reply == ERR_ERRONEUSNICKNAME)
        err_erroneusnickname(server, user);
    else if (reply == ERR_NICKNAMEINUSE)
        err_nicknameinuse(server, user);
    else if (reply == ERR_UNKNOWNCOMMAND)
        err_unknowncommand(server, user);
}

std::string createMessage(Server &server, User &user, Code Code)
{
    std::string from = server.getServerName();
    std::string to = user.getNickname();
    std::string msg = ":" + from + " " + to_string(Code) + " " + to + " : ";
    return msg;
}

std::string err_unknowncommand(Server &server, User &user)
{
    return (createMessage(server, user, ERR_UNKNOWNCOMMAND) + "Unknown command");
}

std::string err_nonicknamegiven(Server &server, User &user)
{
    return (createMessage(server, user, ERR_NONICKNAMEGIVEN) + "No nickname given");
}

std::string err_erroneusnickname(Server& server, User& user)
{
    return createMessage(server, user, ERR_ERRONEUSNICKNAME) + "Erroneus nickname";
}

std::string err_nicknameinuse(Server& server, User& user)
{
    return createMessage(server, user, ERR_NICKNAMEINUSE) + "Nickname collision";
}

std::string rlp_nickok(Server &server, User &user)
{
    return createMessage(server, user, RLP_NICKOK) + "Nick succesfullty changed";
}