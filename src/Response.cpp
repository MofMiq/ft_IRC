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

std::string errNosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOSUCHCHANNEL, user, cmd) + channelName + " : No such channel\n");
}

std::string errUnknowncommand(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_UNKNOWNCOMMAND, user, cmd) + ": Unknown command\n");
}

std::string errNonicknamegiven(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_NONICKNAMEGIVEN, user, cmd) + ": No nickname given\n");
}

std::string errErroneusnickname(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_ERRONEUSNICKNAME, user, cmd) + ": Erroneus nickname\n");
}

std::string errNicknameinuse(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NICKNAMEINUSE, user, cmd) + ": Nickname is already in use\n");
}

std::string errNotonchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOTONCHANNEL, user, cmd) + channelName + ": You're not on that channel\n");
}

std::string errUsernotinchannel(Server& server, User& user, Command &cmd, const std::string& targetNickname, const std::string& channelName)
{
    return (createMessage(server, ERR_USERNOTINCHANNEL, user, cmd) + targetNickname + " " + channelName + ": They aren't on that channel");
}

std::string errNeedmoreparams(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NEEDMOREPARAMS, user, cmd) + ": Not enough parameters\n");
}

std::string errChanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_CHANOPRIVSNEEDED, user, cmd) + channelName + ": You're not channel operator\n");
}

std::string rplNickok(Server &server, User &user)
{
    return (createReply(server, RPL_NICKOK) + user.getOldNick() + " Nick succesfully changed to " + user.getNickname() + "\n");
}

std::string rplNotopic(Server &server, User &user, const std::string& channelName)
{
    return (createReply(server, RPL_NOTOPIC) + user.getNickname() + " " + channelName + " : No topic is set\n");
}

std::string rplTopic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic)
{
    return (createReply(server, RPL_TOPIC) + user.getNickname() + " " + channelName + " : " + channelTopic + "\n");
}

std::string rplTopicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick)
{
    return (createReply(server, RPL_TOPICWHOTIME) + user.getNickname() + " " + channelName + " " + nick + getTimestamp() + "\n");
}
