#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//#include "Server.hpp"
#include "User.hpp"

class Command;
class Server;

enum Code
{
    RPL_WELCOME = 001,
    RPL_YOURHOST = 002,
    RPL_CREATED = 003,
    RPL_MYINFO = 004,
    RPL_NICKOK = 010,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_TOPICWHOTIME = 333,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_NEEDMOREPARAMS = 461,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_BADCHANNELKEY = 475
};

// the trailer is the post colon (:) message
// the struct of the REPLY is :<FROM> <CODE> <TO> <COMMAND> <PARAMETERS> :<TRAILER>
// the struct of the MESSAGE is :<FROM> <COMMAND> <PARAMETERS> :<TRAILER>

//clase de Response???

std::string getTimestamp();
std::string to_string(int value);

std::string createMessage(Server &server, Code code, User &user, Command &cmd);
std::string createReply(Server &server, Code code);

std::string errNosuchnick(Server &server, User &user, Command &cmd, const std::string& otherNick);
std::string errNosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errCannotsendtochannel(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errUnknowncommand(Server &server, User &user, Command &cmd);
std::string errNonicknamegiven(Server &server, User &user, Command &cmd);
std::string errErroneusnickname(Server& server, User& user, Command &cmd);
std::string errNicknameinuse(Server& server, User& user, Command &cmd);
std::string errUsernotinchannel(Server& server, User& user, Command &cmd, const std::string& targetNickname, const std::string& channelName);
std::string errNotonchannel(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errNeedmoreparams(Server& server, User& user, Command &cmd);
std::string errChanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errBadChannelKey(Server& server, User& user, Command& cmd, const std::string& channelName);
std::string rplWelcome(Server &server, User &user);
std::string rplYourHost(Server &server, User &user);
std::string rplCreated(Server &server, User &user);
std::string rplMyInfo(Server &server, User &user);
std::string rplNickok(Server &server, User &user);
std::string rplNotopic(Server &server, User &user, const std::string& channelName);
std::string rplTopic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic);
std::string rplTopicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick, const std::string& timestamp);


#endif