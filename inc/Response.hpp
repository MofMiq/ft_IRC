#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//#include "Server.hpp"
#include "User.hpp"

class Command;
class Server;
class Channel;

enum Code
{
    RPL_WELCOME = 001,
    RPL_YOURHOST = 002,
    RPL_CREATED = 003,
    RPL_MYINFO = 004,
    RPL_CHANNELMODEIS = 324,
    RPL_CREATIONTIME = 329,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_TOPICWHOTIME = 333,
    RPL_INVITING = 341,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHCHANNEL = 403,
    ERR_TOOMANYTARGETS = 407,
    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412, 
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEOUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTERED = 462,
    ERR_PASSWDMISMATCH = 464,
    ERR_CHANNELISFULL = 471,
    ERR_UNKNOWNMODE = 472,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_BADCHANMASK = 476,
    ERR_CHANOPRIVSNEEDED = 482
};

// the trailer is the post colon (:) message
// the struct of the REPLY is :<FROM> <CODE> <TO> <COMMAND> <PARAMETERS> :<TRAILER>
// the struct of the MESSAGE is :<FROM> <COMMAND> <PARAMETERS> :<TRAILER>

//clase de Response???

std::string getTimestamp();
std::string to_string(int value, int n);

std::string createMessage(Server &server, Code code, User &user, Command &cmd);
std::string createReply(Server &server, Code code);

std::string errNosuchnick(Server &server, User &user, Command &cmd, const std::string& otherNick);
std::string errNosuchchannel(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errToomanytargets(Server &server, User &user, Command &cmd);
std::string errNorecipient(Server &server, User &user, Command &cmd);
std::string errNotexttosend(Server &server, User &user, Command &cmd);
std::string errUnknowncommand(Server &server, User &user, Command &cmd);
std::string errNonicknamegiven(Server &server, User &user, Command &cmd);
std::string errErroneousnickname(Server& server, User& user, Command &cmd);
std::string errNicknameinuse(Server& server, User& user, Command &cmd);
std::string errUsernotinchannel(Server& server, User& user, Command &cmd, const std::string& targetNickname, const std::string& channelName);
std::string errNotonchannel(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string errUseronchannel(Server& server, User& user, Command& cmd, const std::string& targetNickname, const std::string& channelName);
std::string errNeedmoreparams(Server& server, User& user, Command &cmd, int i);
std::string errAlreadyRegistered(Server& server, User& user, Command& cmd);
std::string errPasswdmismatch(Server& server, User& user, Command& cmd);
std::string errChannelIsFull(Server& server, User& user, Command& cmd, const std::string& channelName);
std::string errUnknownmode(Server& server, User& user, Command& cmd, const std::string& modechar);
std::string errInviteOnlyChan(Server& server, User& user, Command& cmd, const std::string& channelName);
std::string errBadChannelKey(Server& server, User& user, Command& cmd, const std::string& channelName);
std::string errBadChannelMask(Server& server, User& user, Command& cmd, const std::string& channelName);
std::string errChanoprivsneeded(Server &server, User &user, Command &cmd, const std::string& channelName);
std::string rplWelcome(Server &server, User &user);
std::string rplYourHost(Server &server, User &user);
std::string rplCreated(Server &server, User &user);
std::string rplMyInfo(Server &server, User &user);
std::string rplChannelmodeis(Server &server, User &user, Command& cmd, Channel& channel);
std::string rplCreationtime(Server &server, User &user, Command& cmd, Channel& channel);
std::string rplNotopic(Server &server, User &user, const std::string& channelName);
std::string rplTopic(Server &server, User &user, const std::string& channelName, const std::string& channelTopic);
std::string rplTopicwhotime(Server &server, User &user, const std::string& channelName, const std::string& nick, const std::string& timestamp);
std::string rplInviting(Server& server, User& user, const std::string& targetNickname, const std::string& channelName);
std::string rplNamreply(Server& server, User& user, Command& cmd, Channel& channel);
std::string rplEndofnames(Server& server, User& user, Command& cmd, const std::string& channelName);

#endif