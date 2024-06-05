#include "../inc/Commands.hpp"

/*
NICK: is used to give the client a nickname o change the previous one.
If a NICK command is used with a nickname that is already in use -> ERR_NICKNAMEINUSE numeric and ignore the NICK command

ERR_ERRONEUSNICKNAME: containing invalid characters

Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|)
characters in nicknames, and MAY disallow digits as the first character.

ERR_NONICKNAMEGIVEN: does not receive the <nickname> parameter with the NICK command and ignore

The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other
clients about the change of nickname.

Example:
NICK Wiz                  ; Requesting the new nick "Wiz".
:WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.
:dan-!d@localhost NICK Mamoped
                            ; dan- changed his nickname to Mamoped.

ERR_NICKCOLLISION: Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server) -> not needed?

*/