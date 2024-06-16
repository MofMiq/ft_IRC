#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//#include "Server.hpp"
#include "User.hpp"

class Command;
class Server;

enum Code
{
    // Error code for NICK
    RPL_NICKOK = 002,
    RPL_TOPIC = 332,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NEEDMOREPARAMS = 461
};

// the trailer is the post colon (:) message
// the struct of the REPLY is :<FROM> <CODE> <TO> <COMMAND> <PARAMETERS> :<TRAILER>
// the struct of the MESSAGE is :<FROM> <COMMAND> <PARAMETERS> :<TRAILER>

//clase de Response???

std::string to_string(int value);

void        redirectMessage(Server& server, User& user, Code Code, Command& cmd);

std::string createMessage(Server& server, User& user, Code Code);
std::string createReply(Server &server, User &user, Code Code, Command& cmd);

std::string err_unknowncommand(Server& server, User& user);
std::string err_nonicknamegiven(Server& server, User& user);
std::string err_erroneusnickname(Server& server, User& user);
std::string err_nicknameinuse(Server& server, User& user);
std::string err_needmoreparams(Server& server, User& user);
std::string rpl_nickok(Server &server, User &user, Command& cmd);
std::string rpl_topic(Server &server, User &user, Command& cmd);


#endif