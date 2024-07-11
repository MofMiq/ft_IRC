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
    std::string hostname = cmd.getArg(2); // Este debe ser "0" (segun el manual)
    std::string servername = cmd.getArg(3); // Este debe ser "*" (segun el manual)
    std::string realname = cmd.getArg(4);

    //Error q deberia salir segun manual¿¿??
/*     if (hostname != "0" || servername != "*") // debug cuando se haga el push final hay que descomentarlo
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 5));
        return;
    }
 */
    // Actualizar la informacion del usuario
    user.setUsername(username);
    user.setHostname(hostname);
    user.setServername(servername);
    user.setRealname(realname);

    //Para el control de haber realizado el comando USER
    user.setCommandUSER(true);
    //std::cout << "_commandUSER = " << user.getCommandUSER() << std::endl;
    if (user.getAuthenticated() && user.getCommandNICK() && user.getCommandUSER() && !user.getConfigOK())
    {
        user.setConfigOK(true);
        std::cout << "_configOK = " << user.getConfigOK() << std::endl;        
    }
    // user.enqueueResponse(rplWelcome(server, user));
    // user.enqueueResponse(rplYourHost(server, user));
    // user.enqueueResponse(rplCreated(server, user));
    // user.enqueueResponse(rplMyInfo(server, user));

    server.sendMessageClient(user.getFd(), ":MyServer 001 " + user.getNickname() + " :Welcome to the IRC Network, " + user.getNickname());
    server.sendMessageClient(user.getFd(), ":MyServer 002 " + user.getNickname() + " :Your host is MyServer, running version 1.0");
    server.sendMessageClient(user.getFd(), ":MyServer 003 " + user.getNickname() + " :This server was created at some date");
    server.sendMessageClient(user.getFd(), ":MyServer 004 " + user.getNickname() + " MyServer 1.0 o o");
}