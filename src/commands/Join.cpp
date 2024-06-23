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

void Command::executeJoin(Command& cmd, Server& server, User& user)
{
    // Verifica si se proporcionaron suficientes argumentos
    if (cmd._argCount < 2) {
        /*         // No se proporcionaron suficientes argumentos para el comando JOIN
                std::cout << "JOIN: Insufficient arguments" << std::endl;
                return; */
        user.enqueueResponse(errNeedmoreparams(server, user, cmd));
        //std::cout << user.dequeueResponse();
        server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return;
    }

   // std::string channelName = cmd.getArg(1);

    // Obtener los nombres de los canales y las claves del comando
    std::string channelNames = cmd.getArg(1);
    std::string keys = (cmd._argCount > 2) ? cmd.getArg(2) : ""; //Solo hay keys si hay más de  cmds sino ""

    // Dividir los nombres de los canales y las claves en vector
    std::vector<std::string> channelList = splitString(channelNames, ',');
    std::vector<std::string> keyList = splitString(keys, ',');

    // Procesar cada canal
    for (size_t i = 0; i < channelList.size(); ++i)
    {
        std::string channelName = channelList[i];
        std::string key = (i < keyList.size()) ? keyList[i] : ""; //la key del canal

        // Verificar si el canal ya existe en el servidor
        if (server.channelExists(channelName))
        {
            //// El canal ya existe, verificar la clave del canal
            //if (!server.checkChannelKey(channelName, key)) {
            //    user.enqueueResponse(errBadChannelKey(server, user, cmd, channelName));
            //    std::cout << user.dequeueResponse(); 
            //    continue;
            //}

            // Verifica si el usuario ya esta en el canal
            if (server.isUserInChannelServer(user, channelName)) {
                //NO existe este mensaje, no se si ponerlo
                //user.enqueueResponse(errUserOnChannel(server, user, channelName));
                //std::cout << user.dequeueResponse(); // Ahora mismo es para probar
                std::cout << "User " << user.getNickname() << " NOT JOINED bc is yet inchannel " << channelName << std::endl;
                continue;
            }
            // Agrega al usuario al canal
            server.addUserToChannel(user, channelName);
            std::cout << "User " << user.getNickname() << " joined channel " << channelName << std::endl;
        }
        else
        {
            // El canal no existe, crea el canal y agrega al usuario
           // server.createChannel(channelName, key);
            server.createChannel(channelName);
            server.addUserToChannel(user, channelName);
            server.setOperator(user, channelName); //es operador
            std::cout << "User " << user.getNickname() << " created and joined channel " << channelName << std::endl;
        }
    }
    server.ShowChannelsAndUsers();
    return;
}
