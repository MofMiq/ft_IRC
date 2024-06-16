#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"


Code Command::executeJoin(Command& cmd, Server& server, User& user)
{
    // Verifica si se proporcionaron suficientes argumentos
    if (cmd._argCount < 2) {
/*         // No se proporcionaron suficientes argumentos para el comando JOIN
        std::cout << "JOIN: Insufficient arguments" << std::endl;
        return; */
        return ERR_NEEDMOREPARAMS;
    }

    // Obt�n el nombre del canal del segundo argumento
    std::string channelName = cmd.getArg(1);

    // Verifica si el canal ya existe en el servidor
    if (server.channelExists(channelName)) {
        // El canal ya existe, agrega al usuario al canal
        server.addUserToChannel(user, channelName);
        std::cout << "User " << user.getNickname() << " joined channel " << channelName << std::endl;
    }
    else {
        // El canal no existe, crea el canal y agrega al usuario
        server.createChannel(channelName);
        server.addUserToChannel(user, channelName);
        std::cout << "User " << user.getNickname() << " created and joined channel " << channelName << std::endl;
    }
    return RPL_TOPIC;
}
