#include "../../inc/Command.hpp"

/*
MODE #channel <modes> <parameters>

MODE: is used to set or remove options (or modes) from a given target.
+ is to add a mode; - is to removea mode.  more than one  mode can be add at one time.

    -i: set/remove Invite-only channel
    MODE #channel +i -> you make the channel private
    MODE #channel -i -> you make the channel public

    -t: set/remove the restrictions of the TOPIC command to channel operator
    MODE #channel +t -> only operator can change channel's topic
    MODE #channel -t -> any user can change channel's topic

    -k: set/remove the channel password
    MODE #channel +k <password> -> set password
    MODE #channel -k -> remove password

    -o: give/take operator privilege to another user
    MODE #channel +o <nickname> -> gives operator privilege to the given user
    MODE #channel -o <nickname> -> take operator privilege to the given user
    
    -l: set/remove the user limit to channel
    MODE #channel +l <number> -> limits max number of clients i a channel
    MODE #channel -l -> removes limit

    MODE #channel +itklo <password> <maxClient> <user> -> ok
    MODE #channel +i +t +k +l +o <std:password> <int:maxClient> <str:user> -> ok
    MODE #channel -ikl +o user -> ok, just one +- or -+ good
    MODE #channel +o -ikl user -> bad
    MODE #channel +i-k-o -tl <password> <user> -> good

    RES:
    :servidor 324 tuNick #miCanal +i
    :servidor 324 tuNick #miCanal -t
    :servidor 324 tuNick #miCanal +k miClave
    :servidor 324 tuNick #miCanal +l 50
    :servidor 324 tuNick #miCanal +o juan
    :servidor 324 tuNick #miCanal +itkl 50 miClave +o juan //en el canal
    //notificacion a los usuarios del canal
    :admin!ident@host MODE #miCanal +i
    :admin!ident@host MODE #miCanal +t
    :admin!ident@host MODE #miCanal +k miClave
    :admin!ident@host MODE #miCanal +l 50
    :admin!ident@host MODE #miCanal +o juan

    461 -> MODE solo, o +k+k-o+l sin cuarto arg
    (324) RPL_CHANNELMODEIS: Respuesta general al comando MODE, indicando los modos actuales del canal.
    ERR_INVALIDMODEPARAM (696)
*/

void Command::executeMode(Command& cmd, Server& server, User& user)
{
    std::cout << "hola desde MODE" << std::endl;
    (void)cmd;
    (void)server;
    (void)user;
    return ;
}