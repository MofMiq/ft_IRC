#include "../inc/Response.hpp"

std::string to_string(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string createMessage(Server& server, User& user, ErrorCode errorCode)
{
    std::string from = server.getServerName();
    std::string to = user.getNickname();
    std::string msg = ":" + from + " " + to_string(errorCode) + " " + to + " : ";
    return msg;
}

std::string err_nonicknamegiven(Server& server, User& user)
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