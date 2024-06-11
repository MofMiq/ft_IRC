#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Server.hpp"
#include "User.hpp"


enum ErrorCode
{
    // Error code for NICK
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436 //creo que no hace falta
};

// the trailer is the post colon (:) message
// the struct of the REPLY is :<FROM> <CODE> <TO> <COMMAND> <PARAMETERS> :<TRAILER>
// the struct of the MESSAGE is :<FROM> <COMMAND> <PARAMETERS> :<TRAILER>

//clase de Response???

std::string to_string(int value);

std::string createMessage(Server& server, User& user, ErrorCode errorCode);

std::string err_nonicknamegiven(Server& server, User& user);
std::string err_erroneusnickname(Server& server, User& user);
std::string err_nicknameinuse(Server& server, User& user);


#endif