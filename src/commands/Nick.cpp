#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
/*

NICK <newNick>

NICK: is used to give the client a nickname o change the previous one.
(431)ERR_NONICKNAMEGIVEN: does not receive the <nickname> parameter with the NICK command and ignore

(432)ERR_ERRONEOUSNICKNAME: containing invalid characters:  -, [, ], \, ``, {, }, ^, _, y |

Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|)
characters in nicknames, and MAY disallow digits as the first character.

(433)ERR_NICKNAMEINUSE ->If a NICK command is used with a nickname that is already in use; all errors ignore the NICK command

RESPONSE: The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other
clients about the change of nickname. Ex: oldNick!user@host NICK :newNick

Example:
NICK Wiz                  ; Requesting the new nick "Wiz".
:WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.
:dan-!d@localhost NICK Mamoped
                            ; dan- changed his nickname to Mamoped.

-> not needed? ERR_NICKCOLLISION: Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server)

*/


bool isAllowedSymbol(char c)
{
    if ((c == '[') || (c == ']')
        || (c == '{') || (c == '}') || (c == '\\') || (c == '|') || (c == '_')
        || (c == '-') || (c == '`'))
        return true;
    return false;
}

bool validNickname(std::string& nick)
{
    char c = nick.at(0);

    if (std::isdigit(c) || (!isAllowedSymbol(c) && !isalpha(c)))
        return false;
    for (size_t i = 1; i < nick.length(); i++)
    {
        c = nick.at(i);
        if (!std::isalnum(c) && !isAllowedSymbol(c))
            return false;
    }
    return true;
}

void Command::executeNick(Command &cmd, Server &server, User &user)
{
    if (cmd._argCount < 2 || cmd.getArg(1).length() == 0)
    {
        user.enqueueResponse(errNonicknamegiven(server, user, cmd));
        server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return ;
    }

    if (!validNickname(cmd.getArg(1)) || cmd.getArg(1).length() >= MAX_LENGHT)
    {
        user.enqueueResponse(errErroneousnickname(server, user, cmd));
        server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return ;
    }
    if (server.isNickInServer(cmd.getArg(1)))
    {
        user.enqueueResponse(errNicknameinuse(server, user, cmd));
        server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return ;
    }
    user.setOldNick(user.getNickname());
    user.setNickname(cmd.getArg(1));
    server.updateUsersServerByNick(user.getFd(), cmd.getArg(1));

    user.enqueueResponse(rplNickok(server, user));
    server.sendMessageClient(user.getFd(), user.dequeueResponse());
    sendMessageToChannels(server, user, server.getAllChannelsUserIn(user.getFd()), rplNickok(server, user));
    return ;
}
