#include "../inc/Response.hpp"
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getTimestamp()
{
    std::time_t timestamp = std::time(NULL);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&timestamp));
    std::ostringstream result;
    result << "Timestamp: " << timestamp << " Date: " << buffer;
    return result.str();
}

std::string to_string(int value)
{
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << value;
    return oss.str();
}

//nombre de servidor code y todo lo demas segun cada mensaje

std::string createMessage(Server &server, Code code, User &user, Command &cmd)
{
    std::string msg = ":" + server.getServerName() + " " + to_string(code) + " " + user.getNickname() + " failed to execute " + cmd.getArg(0) + " ";
    return msg;
}

std::string createReply(Server &server, Code code)
{
    std::string msg = ":" + server.getServerName() + " " + to_string(code) + " ";
    return msg;
}

std::string err_nosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOSUCHCHANNEL, user, cmd) + channelName + " : No such channel\n");
}

std::string err_unknowncommand(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_UNKNOWNCOMMAND, user, cmd) + ": Unknown command\n");
}

std::string err_nonicknamegiven(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_NONICKNAMEGIVEN, user, cmd) + ": No nickname given\n");
}

std::string err_erroneusnickname(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_ERRONEUSNICKNAME, user, cmd) + ": Erroneus nickname\n");
}

std::string err_nicknameinuse(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NICKNAMEINUSE, user, cmd) + ": Nickname is already in use\n");
}

std::string err_notonchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOTONCHANNEL, user, cmd) + channelName + ": You're not on that channel\n");
}

std::string err_needmoreparams(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NEEDMOREPARAMS, user, cmd) + ": Not enough parameters\n");
}

std::string err_chanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_CHANOPRIVSNEEDED, user, cmd) + channelName + ": You're not channel operator\n");
}

std::string rpl_nickok(Server &server, User &user)
{
    return (createReply(server, RPL_NICKOK) + user.getOldNick() + " Nick succesfully changed to " + user.getNickname() + "\n");
}

std::string rpl_notopic(Server &server, User &user, const std::string& channelName)
{
    return (createReply(server, RPL_NOTOPIC) + user.getNickname() + " " + channelName + " : No topic is set\n");
}

std::string rpl_topic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic)
{
    return (createReply(server, RPL_TOPIC) + user.getNickname() + " " + channelName + " : " + channelTopic + "\n");
}

std::string rpl_topicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick)
{
    return (createReply(server, RPL_TOPICWHOTIME) + user.getNickname() + " " + channelName + " " + nick + getTimestamp() + "\n");
}
