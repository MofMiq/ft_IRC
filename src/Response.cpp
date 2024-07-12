#include "../inc/Response.hpp"
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/Channel.hpp"
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
    std::string msg;
    if (cmd._argCount > 0)
        msg = ":" + server.getServerName() + " " + to_string(code) + " " + user.getNickname() + " failed to execute " + cmd.getArg(0) + " ";
    else
        msg = ":" + server.getServerName() + " " + to_string(code) + " " + user.getNickname() + " failed to execute an empty command ";
    return msg;
}

std::string createReply(Server &server, Code code)
{
    std::string msg = ":" + server.getServerName() + " " + to_string(code) + " ";
    return msg;
}

std::string errNosuchnick(Server &server, User &user, Command &cmd, const std::string& otherNick)//poner otherNick name al final?
{
    return (createMessage(server, ERR_NOSUCHNICK, user, cmd) + otherNick + ": No such nick");
}

std::string errNosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName)//poner channel name al final?
{
    return (createMessage(server, ERR_NOSUCHCHANNEL, user, cmd) + channelName + " : No such channel");
}

std::string errToomanytargets(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_TOOMANYTARGETS, user, cmd) + ": Too many targets");
}
std::string errNorecipient(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_NORECIPIENT, user, cmd) + ": No recipient given");
}
std::string errNotexttosend(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_UNKNOWNCOMMAND, user, cmd) + ": No text to send");
}
std::string errUnknowncommand(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_UNKNOWNCOMMAND, user, cmd) + ": Unknown command" + "\nThis are the COMMANDS we have available:\nUSER NICK JOIN TOPIC INVITE PRIVMSG KICK MODE (itkol)"); //debug
}

std::string errNonicknamegiven(Server &server, User &user, Command &cmd)
{
    return (createMessage(server, ERR_NONICKNAMEGIVEN, user, cmd) + ": No nickname given");
}

std::string errErroneousnickname(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_ERRONEOUSNICKNAME, user, cmd) + ": Erroneous nickname");
}

std::string errNicknameinuse(Server& server, User& user, Command &cmd)
{
    return (createMessage(server, ERR_NICKNAMEINUSE, user, cmd) + ": Nickname is already in use");
}

std::string errNotonchannel(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_NOTONCHANNEL, user, cmd) + channelName + ": You're not on that channel");
}

std::string errUsernotinchannel(Server& server, User& user, Command &cmd, const std::string& targetNickname, const std::string& channelName)
{
    return (createMessage(server, ERR_USERNOTINCHANNEL, user, cmd) + targetNickname + " " + channelName + ": They aren't on that channel");
}

std::string errUseronchannel(Server& server, User& user, Command& cmd, const std::string& targetNickname, const std::string& channelName)
{
    return (createMessage(server, ERR_USERONCHANNEL, user, cmd) + targetNickname + " " + channelName + ":is already on channel");
}

std::string errNeedmoreparams(Server& server, User& user, Command &cmd, int i)
{
    std::string msg = createMessage(server, ERR_NEEDMOREPARAMS, user, cmd) + ": Not enough parameters";
    if (i == 2)
        msg += "\nUsage: TOPIC <channel> :[new topic]";
    else if (i == 3)
        msg += "\nUsage: PRIVMSG <user>/<channel> :<the message you want to send]>";
    else if (i == 4)
        msg += "\nUsage: MODE <channel> <modestring> <mode arguments>...\nAvailable channel modes: itkol";
    else if (i == 5)
        msg += "\nUsage: USER <username> 0 * <realname>";
    else if (i == 6)
        msg += "\nUsage: INVITE <user> <channel>";
    else if (i == 7)
        msg += "\nUsage: KICK <channel> <user>{,<user>} [:<reason>]";
    else if (i == 8)
        msg += "\nUsage: JOIN <channel>{,<channel>} [<key>{,<key>}]";
    return msg;
}

std::string errAlreadyRegistered(Server& server, User& user, Command& cmd)
{
    return createMessage(server, ERR_ALREADYREGISTERED, user, cmd) + ":You may not reregister";
}

std::string errChanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_CHANOPRIVSNEEDED, user, cmd) + channelName + ": You're not channel operator");
}
std::string errChannelIsFull(Server& server, User& user, Command& cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_CHANNELISFULL, user, cmd) + user.getNickname() + " " + channelName + " :Cannot join channel (+l)");
}

std::string errInviteOnlyChan(Server& server, User& user, Command& cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_INVITEONLYCHAN, user, cmd) + user.getNickname() + " " + channelName + " :Cannot join channel (+i)");
}

std::string errBadChannelKey(Server& server, User& user, Command& cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_BADCHANNELKEY, user, cmd) + user.getNickname() + " " + channelName + ":Cannot join channel (+k)");
}
std::string errBadChannelMask(Server& server, User& user, Command& cmd, const std::string& channelName)
{
    return (createMessage(server, ERR_BADCHANMASK, user, cmd) + channelName + " :Bad Channel Mask");
}

std::string rplWelcome(Server &server, User &user)
{
    return (":" + server.getServerName() + " 001 " + user.getNickname() + ": Welcome to our IRC network :" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname());
}
std::string rplYourHost(Server &server, User &user)
{
    return (":" + server.getServerName() + " 002 " + user.getNickname() + ": Your host is " + user.getServername() + ", running version v1.0");
}
std::string rplCreated(Server &server, User &user)
{
    return (":" + server.getServerName() + " 003 " + user.getNickname() + ": This server was created at " + getTimestamp());
}
std::string rplMyInfo(Server &server, User &user)
{
    return (":" + server.getServerName() + " 004 " + user.getNickname() + " " + user.getServername() + " v1.0 Available user modes: , Available channel modes: itkol");
}

std::string rplChannelmodeis(Server &server, User &user, Command& cmd, Channel& channel, const std::string& extra)
{
    return (createReply(server, RPL_CHANNELMODEIS) + cmd.getArg(0) + " " + user.getNickname() + " " + channel.getName() + extra);
}
std::string rplCreationtime(Server &server, User &user, Command& cmd, Channel& channel)
{
    return (createReply(server, RPL_CREATIONTIME) + cmd.getArg(0) + " " + user.getNickname() + " " + channel.getName() + " " + channel.getCreationTime());
}

std::string rplNotopic(Server &server, User &user, const std::string& channelName)
{
    return (createReply(server, RPL_NOTOPIC) + user.getNickname() + " " + channelName + " : No topic is set");
}

std::string rplTopic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic)
{
    return (createReply(server, RPL_TOPIC) + user.getNickname() + " " + channelName + " " + channelTopic);
}

std::string rplTopicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick, const std::string& timestamp)
{
    return (createReply(server, RPL_TOPICWHOTIME) + user.getNickname() + " " + channelName + " " + nick + " " + timestamp);
}

std::string rplInviting(Server& server, User& user, const std::string& targetNickname, const std::string& channelName)
{
    return (createReply(server, RPL_INVITING) + user.getNickname() + " " + targetNickname + " " + channelName);
}
std::string rplNamreply(Server& server, User& user, Command& cmd, Channel& channel)
{
    return (createReply(server, RPL_NAMREPLY) + cmd.getArg(0) + " " + user.getNickname() + " = " + channel.getName() + " :" + channel.showUsers());
}
std::string rplEndofnames(Server& server, User& user, Command& cmd, const std::string& channelName)
{
    return (createReply(server, RPL_ENDOFNAMES) + cmd.getArg(0) + " " + user.getNickname() + " " + channelName + " :End of name list");
}