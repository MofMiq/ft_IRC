#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <set>
#include "User.hpp"

#define MAXCLI 50 // Número máximo de clientes por defecto

class Channel {
private:
 int _clientCount;
    int _maxClients;
    std::string _name;
    std::string _topic;
    std::string _pass;
    std::map<int, User> _users; // Contenedor de usuarios usando un mapa para acceso rápido por file descriptor
    std::set<int> _operators; // Conjunto de operadores usando file descriptor

    bool _private; // mode -i
    bool _topicPrivate; // mode -t
    bool _passNeeded; // mode -k -> argumento extra para INVITE pero lo establece MODE
    bool _limitClient; // mode -l
    // mode -o no necesita bool porque es la lista de operadores


public:
    // Constructor por defecto
    Channel();

    // Constructor para inicializar el canal con el nombre dado y el número máximo de clientes
    Channel(const std::string& name, int maxClients = MAXCLI);

    ~Channel();

    // Añade un usuario al canal
    void addUser(const User& user);

    // Elimina un usuario del canal
    void removeUser(int userFd);

    // Verifica si un usuario es operador del canal
    bool isUserOperator(int userFd) const;

    // Establece el topic del canal
    void setTopic(const std::string& topic);

    // Obtiene el nombre del canal
    std::string getName() const;

    // Obtiene el topic del canal
    std::string getTopic() const;

    // Verifica si el canal está vacío
    bool isEmpty() const;
};

#endif
