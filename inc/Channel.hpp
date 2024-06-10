#ifndef CHANNEL_HPP
#define CHANNEL_HPP

//#include "Server.hpp"
#include <string>
#include <map>
#include <set>
#include "User.hpp"
#define MAXCLI 50 //default number max client

class Channel
{
    private:
        int     _clientCount;
        int     _maxClients;
        std::string _name;
        std::string _topic;
        std::string _pass;
        std::map<int, User> _users; // Contenedor de usuarios usando un mapa para acceso r�pido por file descriptor
        std::set<int> _operators; // Conjunto de operadores usando file descriptor

        bool _private; //mode -i
        bool _topicPrivate; //mode -t
        bool _passNeedded; // mode -k -> argumento extra para INVITE pero lo establece MODE
        bool _limitClient; //mode -l
        //mode -o no necesita bool por que es la lista de operator

    public:
        //validName() -> lo de comprobar si hay otro comando prohibido

    //Inicializa un nuevo canal con el nombre dado y un n�mero m�ximo de clientes (por defecto MAXCLI).
    //Establece los valores iniciales de los atributos del canal.
        Channel(const std::string& name, int maxClients = MAXCLI);
        ~Channel();

        // M�todos para gestionar el canal

        //A�ade un usuario al canal, insert�ndolo en el mapa _users usando su descriptor de archivo como clave.
        //Incrementa el contador de clientes _clientCount.
        void addUser(const User& user); //INVITE

        //Elimina un usuario del canal, elimin�ndolo del mapa _users y,
        //si es operador, del conjunto _operators. Decrementa el contador de clientes _clientCount.
        void removeUser(int userFd); //KICK

        //Verifica si un usuario, identificado por su descriptor de archivo,
        //es operador del canal. Retorna true si el usuario es operador, false en caso contrario.
        bool isUserOperator(int userFd) const;

        //Establece el topic del canal, actualizando el atributo _topic.
        void setTopic(const std::string& topic);

        //validName() -> lo de comprobar si hay otro comando prohibido

        // Getters y setters
        std::string getName() const;
        std::string getTopic() const;
};

#endif