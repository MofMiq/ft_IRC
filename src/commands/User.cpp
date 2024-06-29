#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

/*
     Command: USER
  Parameters: <username> 0 * <realname>
*/

//void Command::executeUser(Command& cmd, Server& server, User& user)
//{
//    // Verificar que el comando tenga suficientes argumentos
//    if (cmd._argCount < 5)
//    {
//        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 0));
//        return;
//    }
//
//    // Verificar si el usuario ya esta registrado
//    if (!user.getUsername().empty()
//    {
//        user.enqueueResponse(errAlreadyRegistered(server, user, cmd));
//        return;
//    }
//
//    // Extraer y validar los parametros del comando USER
//    std::string username = cmd.getArg(1);
//    std::string param2 = cmd.getArg(2); // Este debe ser "0"
//    std::string param3 = cmd.getArg(3); // Este debe ser "*"
//    std::string realname = cmd.getArg(4);
//
//    if (param2 != "0" || param3 != "*")
//    {
//        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 0));
//        return;
//    }
//
//    // Actualizar la información del usuario
//    user.setUsername(username);
//    user.setRealname(realname);
//
//}