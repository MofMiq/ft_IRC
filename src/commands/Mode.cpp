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

    MODE #channel +itkol <password> <maxClient> <user> -> ok
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

typedef struct Vec
{
	std::vector<std::string> m;
  std::vector<std::string> p;
} vec;

void sendTwoReplies(Server& server, User& user, Command& cmd, Channel& channel, const std::string& extra);
bool isSign(char c);
bool isValidMode(char c);
bool isModeWithParam(std::string& str);
bool onlyNumbers(const std::string& str);
std::string modesChangedReply(Vec& vec, const std::string& key);
Vec lexArgs(Command& cmd);
bool parseArgs(Vec& vec, Command& cmd);
bool applyModes(Vec& vec, Server& server, User& user, Channel& channel);


void Command::executeMode(Command& cmd, Server& server, User& user)
{
	if (cmd._argCount < 2)
	{
		user.enqueueResponse(errNeedmoreparams(server, user, cmd, 4));
	}
	Channel* channel = server.getChannel(cmd.getArg(1));
	if (!server.channelExists(cmd.getArg(1)))
		user.enqueueResponse(errNosuchchannel(server, user, cmd, cmd.getArg(1)));
	else if (cmd._argCount == 2)
		sendTwoReplies(server, user, cmd , *channel, " :Available channel modes: itkol");
	else
	{
		if (!channel->isUserOperator(user.getFd()))
		{
			user.enqueueResponse(errChanoprivsneeded(server, user, cmd, channel->getName()));
			return ;
		}
		Vec vec = lexArgs(cmd);
		if (!parseArgs(vec, cmd) || !applyModes(vec, server, user, *channel))
		{
			sendTwoReplies(server, user, cmd , *channel, " :Available channel modes: itkol");
			return ;
		}
		std::string extra = modesChangedReply(vec, channel->getPass());
		sendTwoReplies(server, user, cmd, *channel, extra);
	}
  return ;
}

bool isSign(char c)
{
	return (c == '+' || c == '-');
}

bool isValidMode(char c)
{
	return (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l');
}

bool isModeWithParam(std::string& str)
{
	return (str == "+l" || str == "+k" || str == "+o" || str == "-o");
}

bool onlyNumbers(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

std::string modesChangedReply(Vec& vec, const std::string& key)
{
	std::string str = " :Channel changed modes: ";
	char c = vec.m[0][0];
	str += std::string(1, c);
	for (size_t i = 0; i < vec.m.size(); i++)
	{
		if (vec.m[i][0] == c)
			str += vec.m[i][1];
		else
		{
			c = vec.m[i][0];
			str += " " + std::string(1, c) + vec.m[i][1];
		}
	}
	for (size_t i = 0; i < vec.p.size(); i++)
	{
		if (vec.p[i] != key)
			str += " " + vec.p[i];
	}
	return str;
}

Vec lexArgs(Command& cmd)
{
	Vec vec;
	for (int i = 2; i < cmd._argCount; i++)
	{
		std::string arg = cmd.getArg(i);
		if (!arg.empty() && isSign(arg[0]))
		{
			if (arg.length() < 3)
				vec.m.push_back(arg);
			else
			{
				char c = arg[0];
				for (size_t j = 1; j < arg.length(); j++)
				{
					if (isSign(arg[j]))
					{
						vec.m.push_back("error");
						break;
					}
					vec.m.push_back(std::string(1, c) + arg[j]);
				}
			}
		}
		else if (!arg.empty())
			vec.p.push_back(arg);
	}
	return vec;
}

bool parseArgs(Vec& vec, Command& cmd)
{
	if (vec.p.size() > vec.m.size())
		return false;
	size_t c = 0;
	for (size_t i = 0; i < vec.m.size(); i++)
	{
		std::string& mstr = vec.m[i];
		if (mstr.length() != 2)
			return false;
		if (isSign(mstr[0]) && isSign(mstr[1]))
			return false;
		if (!isValidMode(mstr[1]))
			return false;
		if (isModeWithParam(mstr))
			c++;
	}
	if (c != vec.p.size())
		return false;
	for (size_t i = 0; i < vec.p.size(); i++)
	{
		std::string& pstr = vec.p[i];
		if (pstr.length() > 10)
			return false;
		for (size_t j = 0; j < pstr.length(); j++)
		{
			if (!std::isalnum(pstr[j]) && !cmd.isAllowedSymbol(pstr[j]))
				return false;
		}
	}
	return true;
}

bool applyModes(Vec& vec, Server& server, User& user, Channel& channel)
{
	size_t j = 0;
	for (size_t i = 0; i < vec.m.size(); i++)
	{
		std::string mode = vec.m[i];
		if (mode == "+i")
			channel.setPrivate(true);
		else if (mode == "-i")
			channel.setPrivate(false);
		else if (mode == "+t")
			channel.setTopicPrivate(true);
		else if (mode == "-t")
			channel.setTopicPrivate(false);
		else if (mode == "+k")
		{
			if (j < vec.p.size() && !vec.p[j].empty())
			{
				channel.setPassNeeded(true);
				channel.setPass(vec.p[j]);
				j++;
			}
			else
				return false;
		}
		else if (mode == "-k")
		{
			channel.setPassNeeded(false);
			channel.setPass("");
		}
		else if (mode == "+o" && j < vec.p.size() && !vec.p[j].empty())
		{
			if (user.getNickname() == vec.p[j])
				return false;
			else if (server.isNickInServer(vec.p[j]) && channel.isUserInChannel(server.getUserByNick(vec.p[j])->getFd()))
			{
				User* user = server.getUserByNick(vec.p[j]);
				channel.addOperatorToChannel(user->getFd());
				j++;
			}
			else
				return false;
		}
		else if (mode == "-o" && j < vec.p.size() && !vec.p[j].empty())
		{
			if (user.getNickname() == vec.p[j])
				return false;
			else if (server.isNickInServer(vec.p[j]) && channel.isUserInChannel(server.getUserByNick(vec.p[j])->getFd()))
			{
				User* user = server.getUserByNick(vec.p[j]);
				channel.removeOperatorToChannel(user->getFd());
				j++;
			}
			else
				return false;
		}
		else if (mode == "+l" && j < vec.p.size() && !vec.p[j].empty())
		{
			if (!onlyNumbers(vec.p[j]))
				return false;
			int aux = std::atoi(vec.p[j].c_str());
			if (aux < 0 && aux > MAX_CLIENTS)
				return false;
			channel.setMaxClient(aux);
			j++;
		}
		else if (mode == "-l")
			channel.setMaxClient(MAX_CLIENTS);
	}
	return true;
}

void sendTwoReplies(Server& server, User& user, Command& cmd, Channel& channel, const std::string& extra)
{
	user.enqueueResponse(rplChannelmodeis(server, user, cmd, channel, extra));
	user.enqueueResponse(rplCreationtime(server, user, cmd, channel));
	std::vector<Channel*> aux;
  aux.push_back(&channel);
	cmd.sendMessageToChannels(user, aux, rplChannelmodeis(server, user, cmd, channel, extra));
	cmd.sendMessageToChannels(user, aux, rplCreationtime(server, user, cmd, channel));
}
