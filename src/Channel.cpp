#include "../inc/Channel.hpp"

//Channel::Channel() : _clientCount(0), _name("") {}

//onstructor por defecto
Channel::Channel() : _clientCount(0), _maxClients(MAXCLI), _name(""), _private(false), _topicPrivate(false),
_passNeeded(false), _limitClient(false) {}

// Constructor para inicializar el canal con el nombre dado y el n�mero m�ximo de clientes
Channel::Channel(const std::string& name, int maxClients)
    : _clientCount(0), _maxClients(maxClients), _name(name), _private(false), _topicPrivate(false),
    _passNeeded(false), _limitClient(false) {}

Channel::~Channel() {}

// A�ade un usuario al canal
void Channel::addUser(const User& user) {
    _users[user.getFd()] = user;
    _clientCount++;
}

// Elimina un usuario del canal
void Channel::removeUser(int userFd) {
    _users.erase(userFd);
    _clientCount--;
    _operators.erase(userFd);
}

// Verifica si un usuario es operador del canal
bool Channel::isUserOperator(int userFd) const {
    return _operators.find(userFd) != _operators.end();
}

// Establece el topic del canal
void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

// Obtiene el nombre del canal
std::string Channel::getName() const {
    return _name;
}

// Obtiene el topic del canal
std::string Channel::getTopic() const {
    return _topic;
}

// Verifica si el canal est� vac�o
bool Channel::isEmpty() const {
    return _users.empty();
}
