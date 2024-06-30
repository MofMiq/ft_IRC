#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

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
	if (cmd._argCount < 2)
	{
		user.enqueueResponse(errNeedmoreparams(server, user, cmd, 4));
	}
	else if (cmd._argCount == 2) //hacer mejor esta mierda
	{
		Channel* tmp = server.getChannel(cmd.getArg(1));
		if (tmp)
		{
			user.enqueueResponse(rplChannelmodeis(server, user, cmd , *tmp, " :Available channel modes: itklo"));
			user.enqueueResponse(rplCreationtime(server, user, cmd, *tmp));
		}
		else
			user.enqueueResponse(errNosuchchannel(server, user, cmd, cmd.getArg(1)));
	}
	else
	{
		if (!server.channelExists(cmd.getArg(1)))
		{
			user.enqueueResponse(errNosuchchannel(server, user, cmd, cmd.getArg(1)));
			return ;
		}
		Channel* channel = server.getChannel(cmd.getArg(1));
		if (!channel->isUserOperator(user.getFd()))
		{
			user.enqueueResponse(errChanoprivsneeded(server, user, cmd, channel->getName()));
		}
		else if (cmd.getArg(2) == "+i")
		{
			channel->setPrivate(true);
			std::cout << RED << "NOW " << channel->getName() << " IS PRIVATE" << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "-i")
		{
			channel->setPrivate(false);
			std::cout << RED << "NOW " << channel->getName() << " IS PUBLIC" << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "+t")
		{
			channel->setTopicPrivate(true);
			std::cout << RED << "NOW " << channel->getName() << " 'S TOPIC IS PRIVATE" << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "-t")
		{
			channel->setTopicPrivate(false);
			std::cout << RED << "NOW " << channel->getName() << " 'S TOPIC IS PUBLIC" << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "+k" && cmd._argCount == 4/*&& !cmd.getArg(3).empty()*/)
		{
			channel->setPassNeeded(true);
			channel->setPass(cmd.getArg(3));
			std::cout << RED << "NOW " << channel->getName() << " HAS A PASSWORD: " << channel->getPass() << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "-k")
		{
			channel->setPassNeeded(false);
			channel->setPass("");
			std::cout << RED << "NOW " << channel->getName() << " DOESN'T HAS A PASSWORD" << END << std::endl;//borrar debug
		}
		else if (cmd.getArg(2) == "+o" && cmd._argCount == 4/*&& !cmd.getArg(3).empty()*/)
		{
			if (channel->isUserInChannel(server.getUserByNick(cmd.getArg(3))->getFd()))
			{
				User* tmp = server.getUserByNick(cmd.getArg(3));
				channel->addOperatorToChannel(tmp->getFd());
				std::cout << RED << "NOW " << tmp->getNickname() << " IS AN OPERATOR OF " << channel->getName() << END << std::endl;//borrar debug
			}
		}
		else if (cmd.getArg(2) == "-o" && cmd._argCount == 4/*&& !cmd.getArg(3).empty()*/)
		{
			if (channel->isUserInChannel(server.getUserByNick(cmd.getArg(3))->getFd()))
			{
				User* tmp = server.getUserByNick(cmd.getArg(3));
				channel->removeOperatorToChannel(tmp->getFd());
				std::cout << RED << "NOW " << tmp->getNickname() << " ISN'T AN OPERATOR OF " << channel->getName() << END << std::endl;//borrar debug
			}
		}
		else if (cmd.getArg(2) == "+l" && cmd._argCount == 4/*&& !cmd.getArg(3).empty()*/)
		{
			int aux = std::atoi(cmd.getArg(3).c_str());
			if (aux > 0 && aux < MAX_CLIENTS)
			{
				channel->setMaxClient(std::atoi(cmd.getArg(3).c_str()));
				std::cout << RED << "NOW " << channel->getName() << " HAS NEW USER'S LIMIT: " << channel->getMaxClient() << END << std::endl;//borrar debug
			}
		}
		else if (cmd.getArg(2) == "-l")
		{
			channel->setMaxClient(MAX_CLIENTS);
			std::cout << RED << "NOW " << channel->getName() << " DOESN'T HAS NEW USER'S LIMIT: " << channel->getMaxClient() << END << std::endl;//borrar debug
		}
	}
  return ;
}