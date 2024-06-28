#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "Response.hpp"
#include "Channel.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define PURPLE "\033[34m"
#define END "\033[0m"

class Server;
class User;

class Command
{

    private:
        std::string                 _fullMessage;
        std::vector<std::string>    _args; // List de comandos prohibidos
        std::vector<std::string>    _prohibitedCommands; // List de comandos prohibidos
        
        Command();

    public:
        int                         _argCount; // numero de parametros que necesita cada comando para funcionar al menos
        Command(const std::string& msg); //en el constructor parametrizado spliteamos el fullMessage en el vector de args
        ~Command();


        std::string& getArg(int i);
        std::string getArgsAsString(int startIdx) const; 
        void sendMessageToChannels(User& user, const std::vector<Channel*>&, const std::string& reply);
        bool isAllowedSymbol(char c);
        //Este m�todo toma un comando en forma de cadena (cmd) y lo convierte en el correspondiente valor enumerado (CMD).
        //Es decir, analiza la cadena de comando y la asigna al enum correspondiente.
        void parseCommand(const std::string& cmd, Server* server, User& user);
        
        //Este m�todo ejecuta un comando especificado (cmd) con los argumentos proporcionados (args).
        //Dependiendo del comando recibido, ejecuta la l�gica correspondiente.
        //Ej: JOIN, -> unirse a un canal,
        //PRIVMSG ->  mensaje privado a otro usuario.
        //ETC
        
        //marina
        void executeNick(Command& cmd, Server& server, User& user);
        void executeTopic(Command& cmd, Server& server, User& user);
        void executePrivmsg(Command& cmd, Server& server, User& user);
        void executeMode(Command& cmd, Server& server, User& user);

        //ana
        void executeJoin(Command& cmd, Server& server, User& user);
        // void executeUser(Command& cmd, Server& server, User& user);
         void executeKick(Command& cmd, Server& server, User& user);
        // void executeInvite(Command& cmd, Server& server, User& user);

};

#endif