#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
    std::set<int> _operators; // Conjunto de operadores usando file descriptor

    bool _private; // mode -i
    bool _topicPrivate; // mode -t
    bool _passNeeded; // mode -k -> argumento extra para INVITE pero lo establece MODE
    // mode -o no necesita bool porque es la lista de operadores

    std::string _timestamp;


public:
    std::map<int, User> _users; // Contenedor de usuarios usando un mapa para acceso rápido por file descriptor
    // Constructor por defecto
    Channel();

    // Constructor para inicializar el canal con el nombre dado y el número máximo de clientes
    Channel(const std::string& name, int maxClients = MAXCLI);

    ~Channel();

    // Añade un usuario al canal
    void addUser(const User& user);

    // Elimina un usuario del canal
    void removeUser(int userFd);

    std::vector<User> getUsers() const;

    // Verifica si un usuario es operador del canal
    bool isUserOperator(int userFd) const;

    // Añade un operador al canal
    void addOperator(int userFd);

    // Establece el topic del canal
    void setTopic(const std::string& topic);

    // Obtiene el nombre del canal
    std::string getName() const;

    // Obtiene el topic del canal
    std::string getTopic() const;

    std::string getPass() const;
    void setPass(const std::string& pass);

    // Verifica si el canal está vacío
    bool isEmpty() const;

    bool isUserInChannel(int userFd) const; 

    bool getPrivate();
    void setPrivate(bool cond);
    bool getTopicPrivate();
    void setTopicPrivate(bool cond);
    bool getPassNeeded();
    void setPassNeeded(bool cond);
    int getMaxClient();
    void setMaxClient(int max);
    std::string getTopicTimestamp();
    void setTopicTimestamp(const std::string& timestamp);

    bool isUserInChannel(int fd);
    bool isUserAnOperators(int fd);
    void addOperatorToChannel(int fd);
    void removeOperatorToChannel(int fd);
};

#endif
