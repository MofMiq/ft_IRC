#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Server.hpp"

enum ErrorCode
{
    // Error code for NICK
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436
};

// the trailer is the post colon (:) message
// the struct of the REPLY is :<FROM> <CODE> <TO> <COMMAND> <PARAMETERS> :<TRAILER>
// the struct of the MESSAGE is :<FROM> <COMMAND> <PARAMETERS> :<TRAILER>

//clase de Response???

#endif