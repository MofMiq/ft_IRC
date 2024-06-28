#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

/*
PRIVMSG <target> [:]<text>

PRIVMSG: is used to send private messages between users and/or channels.
Just one target (user or channel) at time.
: -> para mandar una frase, pero a lo mejor es buna idea incluir los dos puntos siempre

(401) ERR_NOSUCHNICK -> no client can be found
(402) ERR_NOSUCHSERVER -> no server with that name can be found
(403) ERR_NOSUCHCHANNEL -> no channel can be found
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
  if (cmd._argCount < 2)
  {
    user.enqueueResponse(errNeedmoreparams(server, user, cmd, 3));
    return ;
  }
  if (cmd.getArg(1)[0] == ':')
  {
    user.enqueueResponse(errNorecipient(server, user, cmd));
  }
  else if (cmd.getArg(1)[0] != '#' && !server.isNickInServer(cmd.getArg(1)))
  {
    user.enqueueResponse(errNosuchnick(server, user, cmd, cmd.getArg(1)));
  }
  else if (cmd.getArg(1)[0] == '#' && !server.channelExists(cmd.getArg(1)))
  {
    user.enqueueResponse(errNosuchchannel(server, user, cmd, cmd.getArg(1)));
  }
  else if (cmd._argCount == 2)
  {
    user.enqueueResponse(errNeedmoreparams(server, user, cmd, 3));
  }
  else if (cmd._argCount >= 3)
  {
    if (!cmd.getArg(2).empty() && cmd.getArg(2)[0] != ':')
    {
      user.enqueueResponse(errToomanytargets(server, user, cmd));
    }
    else if (cmd.getArgsAsString(2) == ":")
    {
      user.enqueueResponse(errNotexttosend(server, user, cmd));
    }
    else if (cmd.getArg(1)[0] != '#')
    {
      User* rec = server.getUserByNick(cmd.getArg(1));
      rec->enqueueResponse(":" + server.getServerName() + " " + user.getNickname() + " " + getArg(0) + " " + rec->getNickname() + " " + cmd.getArgsAsString(2));  //borrar salto de linea?
    }
    else if (cmd.getArg(1)[0] == '#' && server.getChannel(cmd.getArg(1)))
    {
      std::vector<Channel*> aux;
      aux.push_back(server.getChannel(cmd.getArg(1)));
      std::string reply = ":" + server.getServerName() + " " + user.getNickname() + " " + cmd.getArg(0) + " " + cmd.getArg(1) + " " + cmd.getArgsAsString(2);
      sendMessageToChannels(user, aux, reply);
    }
  }
}
