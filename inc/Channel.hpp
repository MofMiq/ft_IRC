#pragma once

#include "Server.hpp"

class Channel
{
    public:
        int     _clientCount;
        std::string _name;
        std::string _topic;
        std::string _pass;
        //contenedor de users;
        //contenedor de operators;
        bool _passNeedded; // -> argumento extra para INVITE pero lo establece MODE
        //otras tres booleanas para el resto de las  flags



        //Constructores(Client * client en parametrizado -> saber quien es operator)&Destructores
        
        //getter&setter -> TOPIC

        //validName() -> lo de comprobar si hay otro comando prohibido
        //addClient() -> INVITE
        //delClient() -> KICK
};