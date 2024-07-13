#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

/*
TOPIC <channel> [<newTopic>]

TOPIC: is used to change or view the topic of the givel channel.

(331) RPL_NOTOPIC -> 3th argument is not given, if that given channel doesnt have a topic
(332) RPL_TOPIC -> 3th argument is not given, if tha given channel does have a topic
(333) RPL_TOPICWHOTIME -> if 332 is returned, this reply must be send too.

(403) ERR_NOSUCHCHANNEL -> no channel can be found for the given name.
(461) ERR_NEEDMOREPARAMS -> not enough parameters given, in this case at least two  are needed.
(442) ERR_NOTONCHANNEL -> the user sending the command is not joined to the given channel.
(482) ERR_CHANOPRIVSNEEDED -> the 'protected topic' mode is set on channel (mode -t) and a User
without operator privileges tries to change thet topic. But if anyone wants just to display the
topic is good.

RESPONSE: if the topic is changed or cleared, every client in that channel (including the
change author) will recive a TOPIC command with the new topic as argument (or empty if clreared).
If the change is the same as the old one (unchanged), a response must be send anyway.
Ex:
When topic is changed -> :username!ident@host TOPIC #channel :New topic of the channel
When topic is checked ->
:server 332 user #channel :Topic of the channel
:server 333 user #channel userAuthorOfTheChange timestamp

Clients that joins a channel in the future will recive a 332 response.

Example:
TOPIC #chanel :New topic -> Setting the topic on "#test" to "New topic".

TOPIC #test : -> Clearing the topic on "#test"

TOPIC #test -> Checking the topic for "#test"

*/
void enqueueSomeMsgs(Server& server, User& user, Channel& channel);

void Command::executeTopic(Command &cmd, Server &server, User &user)
{
    if (cmd._argCount == 1)
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 2));
        return ;
    }
    std::string channelName = cmd.getArg(1);
    if (!server.channelExists(channelName))
    {
        user.enqueueResponse(errNosuchchannel(server, user, cmd, channelName));
        return;
    }
    Channel* tmp = server.getChannel(channelName);
    if (!tmp->isUserInChannel(user.getFd()))
    {
        user.enqueueResponse(errNotonchannel(server, user, cmd, tmp->getName()));
        return;
    }
    if (tmp->getTopicPrivate() && !tmp->isUserOperator(user.getFd()))
    {
        user.enqueueResponse(errChanoprivsneeded(server, user, cmd, tmp->getName()));
        return ;
    }
    if (cmd._argCount == 2)
    {
        if (tmp->getTopic().empty())
            user.enqueueResponse(rplNotopic(server, user, tmp->getName()));
        else
            enqueueSomeMsgs(server, user, *tmp);
    }
    else if (cmd._argCount >= 3 && cmd.getArg(2)[0] == ':')
    {
        tmp->setTopicTimestamp(getTimestamp());
        std::string actionMessage;
        std::vector<Channel*> aux;
        aux.push_back(server.getChannel(tmp->getName()));
        if (cmd.getArgsAsString(2) == ":")
        {
            tmp->setTopic("");
            actionMessage = " :Topic has been cleared";
        }
        else
        {
            tmp->setTopic(cmd.getArgsAsString(2).erase(0, 1));
            actionMessage = " :Topic has been changed successfully to ";
        }
        enqueueSomeMsgs(server, user, *tmp);
        sendMessageToChannels(user, aux, rplTopic(server, user, tmp->getName(), tmp->getTopic()));
        sendMessageToChannels(user, aux, rplTopicwhotime(server, user, tmp->getName(), user.getNickname(), tmp->getTopicTimestamp()));
        //std::cout << user.getNickname() << actionMessage << tmp->getTopic() << std::endl;
    }
    else
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 2));
    return ;
}

void enqueueSomeMsgs(Server& server, User& user, Channel& channel)
{
    user.enqueueResponse(rplTopic(server, user, channel.getName(), channel.getTopic()));
    user.enqueueResponse(rplTopicwhotime(server, user, channel.getName(), user.getNickname(), channel.getTopicTimestamp()));    
}