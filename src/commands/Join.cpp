#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
/*
  JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar".

  JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                  and &bar using no key.

  JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                  and channel #bar using key "foobar".

  JOIN #foo,#bar                  ; join channels #foo and #bar.
*/

//Devuelve un vector con los strings separados.
//Como se pasan como #canal1,#canal2 juntos hay que separarlos y seguardan en un vector.
//delimiter seria la ,
std::vector<std::string> splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

// Valida el nombre del canal
bool isValidChannelName(const std::string& name)
{
    if (name.empty() || (name[0] != '#' && name[0] != '&'))
        return false;
    if (name.find(':') != std::string::npos || name.find('\n') != std::string::npos)
        return false;
    return true;
}

void Command::executeJoin(Command& cmd, Server& server, User& user) {
    // Verifica si se proporcionan suficientes argumentos
    if (cmd._argCount < 2)
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 8));
        return;
    }

    // Obtener los nombres de los canales y las claves del comando
    std::string channelNames = cmd.getArg(1);
    std::string keys = (cmd._argCount > 2) ? cmd.getArg(2) : ""; //Solo hay keys si hay mas de 2 cmds sino ""

    // Dividir los nombres de los canales y las claves en vector
    std::vector<std::string> channelList = splitString(channelNames, ',');
    std::vector<std::string> keyList = splitString(keys, ',');

    // Procesar cada canal
    for (size_t i = 0; i < channelList.size(); ++i) {
        std::string channelName = channelList[i];
        std::string key = (i < keyList.size()) ? keyList[i] : ""; //la key del canal

        // Validar el nombre del canal
        if (!isValidChannelName(channelName)) {
            user.enqueueResponse(errBadChannelMask(server, user, cmd, channelName));
            continue;
        }

        // Verificar si el canal ya existe en el servidor
        if (server.channelExists(channelName))
        {
                      
            Channel* channel = server.getChannel(channelName);

            // Verificar si el canal est� en modo "solo invitaci�n" y si el usuario no ha sido invitado
            if (channel->getPrivate() && !user.isInvitedToChannel(channelName))
            {
                user.enqueueResponse(errInviteOnlyChan(server, user, cmd, channelName));
                continue;
            }
            //// El canal ya existe, verificar la clave del canal
            if (!server.checkChannelKey(channelName, key))
            {
                user.enqueueResponse(errBadChannelKey(server, user, cmd, channelName));
                continue;
            }
            // Verifica si el canal esta lleno
            if (channel->isFull()) {
                user.enqueueResponse(errChannelIsFull(server, user, cmd, channelName));
                continue;
            }
            // Verifica si el usuario ya esta en el canal
            if (channel->isUserInChannel(user.getFd())) {
                std::cout << "User " << user.getNickname() << " NOT JOINED because already in channel " << channelName;
                continue;
            }
            // Agrega al usuario al canal
            channel->addUser(&user);
            //;
            // Enviar el tema del canal si existe
            if (!channel->getTopic().empty()) {
                user.enqueueResponse(rplTopic(server, user, channel->getName(), channel->getTopic()));
            }
            // Notificar a todos los usuarios del canal
            std::string joinMessage = ":" + user.getNickname() + " JOIN " + channelName;
             // Esto es para notificar a los usuarios del canal sobre la Union
            user.enqueueResponse(joinMessage);
            user.enqueueResponse(rplNamreply(server, user, cmd, *channel));
            user.enqueueResponse(rplEndofnames(server, user, cmd, channel->getName()));
            std::vector<Channel*> aux;
            aux.push_back(server.getChannel(channel->getName()));
            sendMessageToChannels(user, aux, joinMessage);
        }
        else
        {
            // El canal no existe, crea el canal y agrega al usuario
            server.createChannel(channelName, key, getTimestamp());
            server.addUserToChannel(user, channelName);
            Channel* c = server.getChannel(channelName);
            server.setOperator(user, channelName); // es operador
            // Notificar al nuevo usuario que se ha unido al canal
            std::string joinMessage = ":" + user.getNickname() + " JOIN " + channelName;
            user.enqueueResponse(joinMessage);
            user.enqueueResponse(rplNamreply(server, user, cmd, *c));
            user.enqueueResponse(rplEndofnames(server, user, cmd, c->getName()));
        }
    }
}
