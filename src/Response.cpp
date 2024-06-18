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

std::string createMessage(Server &server, Code code, User &user, Command &cmd)
{
    std::string from = server.getServerName();
    std::string msg = ":" + from + " " + to_string(code) + " " + user.getNickname() + " failetd to execute " + cmd.getArg(0);
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

std::string err_nosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOSUCHCHANNEL, user, cmd) + " " + channelName + " : No such channel\n");
}

std::string err_unknowncommand(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_UNKNOWNCOMMAND, user, cmd) + " : Unknown command\n");
}

std::string err_nonicknamegiven(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_NONICKNAMEGIVEN, user, cmd) + " : No nickname given\n");
}

std::string err_erroneusnickname(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_ERRONEUSNICKNAME, user, cmd) + " : Erroneus nickname\n");
}

std::string err_nicknameinuse(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NICKNAMEINUSE, user, cmd) + " : Nickname is already in use\n");
}

std::string err_notonchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOTONCHANNEL, user, cmd) + " " + channelName + " : You're not on that channel\n");
}

std::string err_needmoreparams(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NEEDMOREPARAMS, user, cmd) + ": Not enough parameters\n");
}

std::string err_chanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_CHANOPRIVSNEEDED, user, cmd) + " " + channelName + " : You're not channel operator\n");
}

std::string rpl_nickok(Server &server, User &user, Command& cmd)
{
    return (createMessage(server, RPL_NICKOK, user , cmd) + " " + user.getOldNick() + " Nick succesfullty changed to" + user.getNickname() + "\n");
}

std::string rpl_notopic(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, RPL_NOTOPIC, user, cmd) + " " + channelName + " : No topic is set\n");
}

std::string rpl_topic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic)
{
    std::string msg = user.getNickname() + " " + channelName + " : " + channelTopic + "\n";
    return (createMessage(server, RPL_NOTOPIC, user, cmd) + " " + channelName + " : No topic is set\n");
}

std::string rpl_topicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick)
{
    (void)server;
    std::string msg = user.getNickname() + " " + channelName + " " + nick + " timestamp\n";
    return msg;
}
