#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

/*
     Command: USER
  Parameters: <username> 0 * <realname>
*/

void Command::executeUser(Command& cmd, Server& server, User& user)
{
    // Verificar que el comando tenga suficientes argumentos
    if (cmd._argCount < 5)
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 5));
        return;
    }

    // Verificar si el usuario ya esta registrado
    //If a client tries to send the USER command after they have already completed registration with the server, 
    //the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.
    if (!user.getUsername().empty())
    {
        user.enqueueResponse(errAlreadyRegistered(server, user, cmd));
        return;
    }

    // Extraer y validar los parametros del comando USER
    std::string username = cmd.getArg(1);
    std::string param2 = cmd.getArg(2); // Este debe ser "0" (segun el manual)
    std::string param3 = cmd.getArg(3); // Este debe ser "*" (segun el manual)
    std::string realname = cmd.getArg(4);

    //Error q deberia salir segun manual¿¿??
    if (param2 != "0" || param3 != "*")
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 5));
        return;
    }

    // Actualizar la informacion del usuario
    user.setUsername(username);
    user.setRealname(realname);

    //BORRAR ??
    std::cout << "DATOS COMPLETOS DEL USUARIO CON FD -> " << user.getFd() << std::endl;
    std::cout << "USERNAME -> " << user.getUsername() << std::endl;
    //std::cout << "HOSTNAME -> " << user.getHostname() << std::endl;
    //std::cout << "SERVERNAME -> " << user.getServername() << std::endl;
    std::cout << "REALNAME -> " << user.getRealname() << std::endl;
}