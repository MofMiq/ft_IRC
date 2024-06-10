#pragma once

//#include "Server.hpp"
#include <string>
#include <map>
#include <vector>

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

    private:
        std::map<std::string, CMD> _cmdMap; // Map de strings a enum CMD
        std::map<CMD, int> _argCount; // Map de enum CMD a n�mero de argumentos
        std::vector<std::string> _prohibitedCommands; // List de comandos prohibidos
        int     _argCount; // numero de parametros que necesita cada comando para funcionar al menos

    public:
        Command();
        ~Command();

        //Este m�todo toma un comando en forma de cadena (cmd) y lo convierte en el correspondiente valor enumerado (CMD).
        //Es decir, analiza la cadena de comando y la asigna al enum correspondiente.
        CMD parseCommand(const std::string& cmd) const;
        
        //Este m�todo ejecuta un comando especificado (cmd) con los argumentos proporcionados (args).
        //Dependiendo del comando recibido, ejecuta la l�gica correspondiente.
        //Ej: JOIN, -> unirse a un canal,
        //PRIVMSG ->  mensaje privado a otro usuario.
        //ETC
        void executeCommand(CMD cmd, const std::vector<std::string>& args);
        //metodos
            //parseo de cada uno de los comandos
            //ejecucion de cada uno de los comandos (&server)
};