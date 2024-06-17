#include "../inc/Response.hpp"
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

std::string to_string(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

//nombre de servidor code y todo lo demas segun cada mensaje

std::string createMessage(Server &server, User &user, Code code)
{
    std::string msg = "";
    std::string from = server.getServerName();
    std::string to = user.getNickname();
    msg = ":" + from + " " + to_string(code) + " " + to + " : ";
    return msg;
}

std::string createReply(Server &server, User &user, Code code, Command& cmd)
{
    (void)server;
    (void)code;
    std::string msg = "";
    std::string from = user.getOldNick();
    std::string to = user.getNickname();
    msg = ": " + from + "!" + user.getUsername() + "@" + cmd.getArg(0) + " " + to;
    return msg;
}

std::string err_nosuchchannel(Server &server, User &user, const std::string& channelName)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + " : No such channel";
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
    return (createMessage(server, user, ERR_NICKNAMEINUSE) + "Nickname is already in use" + END);
}

std::string err_notonchannel(Server &server, User &user, const std::string& channelName)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + ": You're not on that channel";
    return msg;
}

std::string err_needmoreparams(Server& server, User& user)
{
    return (createMessage(server, user, ERR_NEEDMOREPARAMS) + "Not enough parameters" + END);
}

std::string err_chanoprivsneeded(Server &server, User &user, const std::string& channelName)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + ": You're not channel operator";
    return msg;
}

std::string rpl_nickok(Server &server, User &user, Command& cmd)
{
    return (createReply(server, user, RPL_NICKOK, cmd) + ": Nick succesfullty changed" + END);
}

std::string rpl_notopic(Server &server, User &user, const std::string& channelName)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + " : No topic is set";
    return msg;
}

std::string rpl_topic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + " : " + channelTopic;
    return msg;
}

std::string rpl_topicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + " " + nick + " timestamp";
    return msg;
}
