#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Response.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/User.hpp"

/* KICK <canal> <usuario> [:<razón>]
<canal> es el nombre del canal del cual se quiere expulsar al usuario.
<usuario> es el nick del usuario que se quiere expulsar.
:<razón> (opcional) es un mensaje opcional que describe la razón de la expulsión.*/


void Command::executeKick(Command& cmd, Server& server, User& user) {
    // Verifica si se proporcionaron suficientes argumentos
    if (cmd._argCount < 3) {
        user.enqueueResponse(err_needmoreparams(server, user));
        return;
    }

    std::string channelName = cmd.getArg(1);
    Channel* channel = server.getChannel(channelName); //TODO

    // Verifica si el canal existe
    if (channel == NULL) {
        user.enqueueResponse(err_nosuchchannel(server, user, channelName));
        return;
    }

    // Verifica si el usuario que ejecuta el comando es un operador del canal
    if (!channel->isUserOperator(user.getFd())) {
        user.enqueueResponse(err_chanoprivsneeded(server, user, channelName));
        return;
    }

    std::string targetNickname = cmd.getArg(2);
    User targetUser = server.getUserByNick(targetNickname); // TODO

    // Verifica si el usuario objetivo está en el canal
    if (!channel->isUserInChannel(targetUser.getFd())) { //TODO
        //user.enqueueResponse(err_usernotinchannel(server, user, targetNickname, channelName));
        return;
    }

    // Obtén la razón para el KICK, si hay alguna
    std::string reason = cmd.getArgsAsString(3); //TODO
    if (reason.empty()) {
        reason = "No reason given"; // Mensaje por defecto si no se proporciona razón
    }

    // Elimina al usuario objetivo del canal
    channel->removeUser(targetUser.getFd());

    // Crear mensajes de confirmación e información para ambos usuarios
    std::string kickMessage = user.getNickname() + " has kicked " + targetNickname + " from channel " + channelName + " : " + reason + "\n";
    user.enqueueResponse(kickMessage);
    targetUser.enqueueResponse(kickMessage);
}


//// Cambia el nickname de un usuario
//void Server::changeUserNick(int userFd, const std::string& newNick) {
//    User& user = _users[userFd];
//    _nickToUser.erase(user.getNickname()); // Elimina el viejo nickname del mapa
//    user.setNickname(newNick); // Actualiza el nickname del usuario
//    _nickToUser[newNick] = user; // Añade el nuevo nickname al mapa
//}