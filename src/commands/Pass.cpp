#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

void Command::executePass(Command& cmd, Server& server, User& user)
{
  if (cmd._argCount < 2)
  {
    user.enqueueResponse(errNeedmoreparams(server, user, cmd, 0));
    return ;
  }
  if (user.getAuthenticated())
  {
    user.enqueueResponse(errAlreadyRegistered(server, user, cmd));
    return ;
  }

  user.setPass(cmd.getArg(1));
}