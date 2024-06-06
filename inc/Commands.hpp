#pragma once

#include "Server.hpp"

enum CMD
{
    NICK,
    USER,
    JOIN,
    PRIVMSG,
    TOPIC,
    KICK,
    INVITE,
    MODE
};


class Command
{
    public:
        //map que asocie los strings con el enum CMD
        //map que tenga clave (Enum) y valor (argumentos del comando)
        //lista de strings de comandos "prohibidos" -> esto sirve para establecer nombres y tal
        int     _argCount; // numero de parametros que necesita cada comando para funcionar al menos
    //metodos
        //parseo de cada uno de los comandos
        //ejecucion de cada uno de los comandos (&server)
};