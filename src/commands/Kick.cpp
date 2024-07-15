#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Response.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/User.hpp"

/* KICK <canal> <usuario> [:<razón>]
<canal> es el nombre del canal del cual se quiere expulsar al usuario.
<usuario> es el nick del usuario que se quiere expulsar.
:<razón> (opcional) es un mensaje opcional que describe la razón de la expulsión.

Expulsar usuario de canal:
KICK #general John :Breaking rules

Expulsar mas de 1 usuario:
KICK #general John,Jane :Breaking rules

Sin comentario:
KICK #general John
KICK #general John,Jane
*/

void Command::executeKick(Command& cmd, Server& server, User& user) {
    // Verifica si se proporcionaron suficientes argumentos
    if (cmd._argCount < 3)
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 7));
        return;
    }
    std::cout << "arg count: " <<cmd._argCount << std::endl;
    std::cout << "arg count: " << cmd.getArg(0) << std::endl;
    std::cout << "arg count: " << cmd.getArg(1) << std::endl;
    std::cout << "arg count: " << cmd.getArg(2) << std::endl;
    std::cout << "arg count: " << cmd.getArg(3) << std::endl;
    // Obtener el nombre del canal y los usuarios objetivo
    std::string channelName = cmd.getArg(1);
    std::string usersStr = cmd.getArg(2);
    //La razon solo si hay
    // getArgsAsString para pillar todos el resto de parametros que contiene el msj
    std::string reason = (cmd._argCount > 3) ? cmd.getArgsAsString(3) : "Kicked by operator";

    // Verifica si el canal existe
    if (!server.channelExists(channelName)) {
        user.enqueueResponse(errNosuchchannel(server, user, cmd, channelName));
        return;
    }

    Channel* channel = server.getChannel(channelName);
    // Verifica si el usuario que envia el comando es operador del canal
    if (!channel->isUserOperator(user.getFd())) {
        user.enqueueResponse(errChanoprivsneeded(server, user, cmd, channelName));
        return;
    }

    // Divide la lista de user
    //while pa cada user
    std::stringstream ss(usersStr);
    std::string targetNickname;
    while (std::getline(ss, targetNickname, ','))
    {
        if (targetNickname == user.getNickname())
        {
            user.enqueueResponse("You cannot kick yourself from the channel !!!");
            continue;
        }
        // Verifica si el usuario está en el canal
        User* targetUser = server.getUserByNick(targetNickname);
        if (targetUser == NULL || !channel->isUserInChannel(targetUser->getFd()))
        {
            user.enqueueResponse(errUsernotinchannel(server, user, cmd, targetNickname, channelName));
            continue;
        }
        // Elimina al usuario del canal 
        channel->removeUser(targetUser->getFd());
        std::string kickMessage = ":" + user.getNickname() + " KICK " + channelName + " " + targetNickname + " :" + reason;

        targetUser->enqueueResponse(kickMessage);
        user.enqueueResponse(kickMessage);
        std::vector<Channel*> aux;
        aux.push_back(server.getChannel(channel->getName()));
        sendMessageToChannels(user, aux, kickMessage);
    }
    server.ShowChannelsAndUsers();
}
