#include "../../inc/Commands.hpp"
#include "../../inc/Response.hpp"

/*

NICK <newNick>

NICK: is used to give the client a nickname o change the previous one.
(431)ERR_NONICKNAMEGIVEN: does not receive the <nickname> parameter with the NICK command and ignore

(432)ERR_ERRONEUSNICKNAME: containing invalid characters:  -, [, ], \, ``, {, }, ^, _, y |

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

//pasrseNick()
//execNick()

bool validNickname(std::string& nick)
{
    char c = nick.at(0);
    if (std::isdigit(c))
        return false;
    return true;
}

void Command::executeNick(Command &cmd, Server &server, User &user)
{
    std::cout << "hola desde NICK" << std::endl;

    if (cmd._argCount < 2 || cmd.getArg(1).length() == 0)
    {
        std::cout << err_nonicknamegiven(server, user) << std::endl;
        //este mensaje se lo tengo que enviar a pespinos/servidor
    }

    if (!validNickname(cmd.getArg(1)))
    {
        std::cout << err_nonicknamegiven(server, user) << std::endl;
    }
    //validNickname
    //check is already exist in the server container of Users
}
