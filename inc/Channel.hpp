#pragma once

#include "Server.hpp"

#define MAXCLI 50 //default number max client

class Channel
{
    public:
        int     _clientCount;
        int     _maxClients;
        std::string _name;
        std::string _topic;
        std::string _pass;

        //contenedor de users;
        //contenedor de operators;
        bool _private; //mode -i
        bool _topicPrivate; //mode -t
        bool _passNeedded; // mode -k -> argumento extra para INVITE pero lo establece MODE
        bool _limitClient; //mode -l
        //mode -o no necesita bool por que es la lista de operator

        //Constructores(Client * client en parametrizado -> saber quien es operator)&Destructores
        
        //getter&setter -> TOPIC

        //validName() -> lo de comprobar si hay otro comando prohibido
        //addClient() -> INVITE
        //delClient() -> KICK
};