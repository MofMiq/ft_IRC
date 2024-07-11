#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"


/*
   INVITE <nickname> <channel>
   INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar
   INVITE xula #example

*/

void Command::executeInvite(Command& cmd, Server& server, User& user) {
    // Verifica si se han proporcionado suficientes argumentos
    if (cmd._argCount < 3)
    {
        user.enqueueResponse(errNeedmoreparams(server, user, cmd, 6));
       // server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return;
    }

    // Obtener el nickname del usuario y el nombre del canal
    std::string targetNickname = cmd.getArg(1);
    std::string channelName = cmd.getArg(2);

    // Verifica si el canal existe
    if (!server.channelExists(channelName))
    {
        user.enqueueResponse(errNosuchchannel(server, user, cmd, channelName));
      //  server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return;
    }

    Channel* channel = server.getChannel(channelName);

    // Verifica si el usuario que envia el comando es miembro del canal
    if (!channel->isUserInChannel(user.getFd()))
    {
        user.enqueueResponse(errNotonchannel(server, user, cmd, channelName));
       // server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return;
    }

    // Verifica si el canal tiene modo de solo invitacion y si el usuario no es operador del canal
    //Si el canal es privado solo lo puede invitar el operador
    if (channel->getPrivate() && !channel->isUserOperator(user.getFd()))
    {
        user.enqueueResponse(errChanoprivsneeded(server, user, cmd, channelName));
      //  server.sendMessageClient(user.getFd(), user.dequeueResponse());
        return;
    }

    //Verifica si el user existe
    User* targetUser = server.getUserByNick(targetNickname);
    if (!targetUser)
    {
        user.enqueueResponse(errNosuchnick(server, user, cmd, targetNickname));
        return;
    }
    // Verifica si el usuario objetivo ya esta en el canal
    if (channel->isUserInChannel(targetUser->getFd()))
    {
        user.enqueueResponse(errUseronchannel(server, user, cmd, targetNickname, channelName));
        return;
    }
    //Se puedee invitar, añadir al vector de invitados
    targetUser->addInvitedChannel(channelName);
    // Envia una respuesta de invitacion exitosa al usuario que envi� el comando
    user.enqueueResponse(rplInviting(server, user, targetNickname, channelName));
   // server.sendMessageClient(user.getFd(), user.dequeueResponse());

    // Envia el mensaje de invitaci�n al usuario objetivo
    std::string inviteMessage = ":" + user.getNickname() + " INVITE " + targetUser->getNickname() + " " + channelName + ":Invite to " + channelName;
   // server.sendMessageClient(targetUser->getFd(), inviteMessage);
    targetUser->enqueueResponse(inviteMessage);

    // Mostrar canales y usuarios para verificar el estado del servidor
    server.ShowChannelsAndUsers();
}