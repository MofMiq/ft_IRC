#include "../inc/Channel.hpp"

//Channel::Channel() : _clientCount(0), _name("") {}

//onstructor por defecto
Channel::Channel() : _clientCount(0), _maxClients(MAXCLI), _name(""), _topic(""), _pass(""), _private(false), _topicPrivate(false),
_passNeeded(false), _timestamp("") {}

// Constructor para inicializar el canal con el nombre dado y el n�mero m�ximo de clientes
Channel::Channel(const std::string& name, int maxClients)
    : _clientCount(0), _maxClients(maxClients), _name(name), _topic(""), _pass(""), _private(false), _topicPrivate(false),
    _passNeeded(false), _timestamp("") {}

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

std::vector<User> Channel::getUsers() const {
    std::vector<User> userList;
    for (std::map<int, User>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
        userList.push_back(it->second);
    }
    return userList;
}

// Verifica si un usuario es operador del canal
bool Channel::isUserOperator(int userFd) const {
    return _operators.find(userFd) != _operators.end();
}

void Channel::addOperator(int userFd) {
    _operators.insert(userFd);
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

std::string Channel::getPass() const
{
    return this->_pass;
}

void Channel::setPass(const std::string& pass)
{
    this->_pass = pass;
}

// Verifica si el canal esta vacio
bool Channel::isEmpty() const {
    return _users.empty();
}

// Función para verificar si el canal esta lleno
bool Channel::isFull() const {
    return _users.size() >= static_cast<size_t>(_maxClients);
}

// Verifica si un usuario está en el canal
bool Channel::isUserInChannel(int userFd) const {
    return _users.find(userFd) != _users.end();
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

int Channel::getMaxClient()
{
    return this->_maxClients;
}

void Channel::setMaxClient(int max)
{
    this->_maxClients = max;
}

std::string Channel::getTopicTimestamp()
{
    return this->_timestamp;
}
void Channel::setTopicTimestamp(const std::string& timestamp)
{
    this->_timestamp = timestamp;
}

bool Channel::isUserInChannel(int fd)
{
    std::map<int, User>::const_iterator it = this->_users.find(fd);
    if (it != this->_users.end())
        return true;
    return false;
}

bool Channel::isUserAnOperators(int fd)
{
    std::set<int>::const_iterator it = this->_operators.find(fd);
    if (it != this->_operators.end())
        return true;
    return false;
}

void Channel::addOperatorToChannel(int fd)
{
   this->_operators.insert(fd);
}

void Channel::removeOperatorToChannel(int fd)
{
   this->_operators.erase(fd);
}