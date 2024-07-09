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

bool validNickname(Command& cmd, std::string& nick)
{
    char c = nick.at(0);

    if (std::isdigit(c) || (cmd.isAllowedSymbol(c) && !isalpha(c)))
        return false;
    for (size_t i = 1; i < nick.length(); i++)
    {
        c = nick.at(i);
        if (!std::isalnum(c) && !cmd.isAllowedSymbol(c))
            return false;
    }
    return true;
}

void Command::executeNick(Command &cmd, Server &server, User &user)
{
    if (cmd._argCount < 2)
    {
        user.enqueueResponse(errNonicknamegiven(server, user, cmd));
    }
    else if (!validNickname(cmd, cmd.getArg(1)) || cmd.getArg(1).length() >= MAX_LENGHT || cmd.getArg(1) == "bot")
    {
        user.enqueueResponse(errErroneousnickname(server, user, cmd));
    }
    else if (server.isNickInServer(cmd.getArg(1)))
    {
        user.enqueueResponse(errNicknameinuse(server, user, cmd));
    }
    else
    {
        user.setOldNick(user.getNickname());
        user.setNickname(cmd.getArg(1));
        server.updateUsersServerByNick(user.getFd(), cmd.getArg(1));

        std::cout << "Client: " << user.getOldNick() << " Command: NICK Arguments: " << user.getNickname() << std::endl;
        std::cout << "Client: " << user.getOldNick() << " changed succesfully their nickname to " << user.getNickname()<< std::endl;
        //std::string rpl = ":" + server.getServerName() + " " + user.getOldNick() + " Nick succesfully changed to " + user.getNickname();
        std::string rpl = ":" + user.getOldNick() + " NICK " + user.getNickname();
        user.enqueueResponse(rpl);
        sendMessageToChannels(user, server.getAllChannelsUserIn(user.getFd()), rpl);
    }
    return ;
}
