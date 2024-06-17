#include "../../inc/Command.hpp"

/*
PRIVMSG <target> [:]<text>

PRIVMSG: is used to send private messages between users and/or channels.
Just one target (user or channel) at time.
: -> para mandar una frase, pero a lo mejor es buna idea incluir los dos puntos siempre

(401) ERR_NOSUCHNICK -> no client can be found
(402) ERR_NOSUCHSERVER -> no server with that name can be found
(403) ERR_NOSUCHCHANNEL -> no channel can be found
(404) ERR_CANNOTSENDTOCHAN -> Indicates that the PRIVMSG could not be delivered to channel. 
This is generally sent in response to channel modes, such as a channel being moderated(nop) and the client not having permission to speak on the channel(?), or not being joined to a channel with the no external messages mode set(?).
(407) ERR_TOOMANYTARGETS -> more than one target
(411) ERR_NORECIPIENT -> no target given
(412) ERR_NOTEXTTOSEND -> no text given

RESPONSE: 
:Angel PRIVMSG Wiz :Hello are you receiving this message ? -> Message from Angel to Wiz.

Ex:
PRIVMSG Angel :yes I'm receiving it ! -> Command to send a message to user Angel.
PRIVMSG #general :yes I'm receiving it ! -> Command to send a message to channel #general.

*/

void Command::executePrivmsg(Command& cmd, Server& server, User& user)
{
    std::cout << "hola desde PRIVMSG" << std::endl;
    (void)cmd;
    (void)server;
    (void)user;
    return ;
}