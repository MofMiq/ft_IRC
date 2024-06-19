#include "../inc/Channel.hpp"

//Channel::Channel() : _clientCount(0), _name("") {}

//onstructor por defecto
Channel::Channel() : _clientCount(0), _maxClients(MAXCLI), _name(""), _topic(""), _private(false), _topicPrivate(false),
_passNeeded(false), _limitClient(false) {}

// Constructor para inicializar el canal con el nombre dado y el n�mero m�ximo de clientes
Channel::Channel(const std::string& name, int maxClients)
    : _clientCount(0), _maxClients(maxClients), _name(name), _topic(""), _private(false), _topicPrivate(false),
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

bool Channel::getPrivate()
{
    return this->_private;
}

void Channel::setPrivate(bool cond)
{
    this->_private = cond;
}

bool Channel::getTopicPrivate()
{
    return this->_topicPrivate;
}

void Channel::setTopicPrivate(bool cond)
{
    this->_topicPrivate = cond;
}

bool Channel::getPassNeeded()
{
    return this->_passNeeded;
}

void Channel::setPassNeeded(bool cond)
{
    this->_passNeeded = cond;
}

bool Channel::getLimitClient()
{
    return this->_limitClient;
}

void Channel::setLimitClient(bool cond)
{
    this->_limitClient = cond;
}

int Channel::getMaxClient()
{
    return this->_maxClients;
}

void Channel::getMaxClient(int max)
{
    this->_maxClients = max;
}
