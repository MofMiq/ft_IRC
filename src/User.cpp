#include "../inc/User.hpp"

User::User()
{
}

User::User(int fd) : _fd(fd)
{
}

User::User(int fd, std::string userName, std::string hostName, std::string serverName, std::string realName) : _fd(fd), _userName(userName), _hostName(hostName), _serverName(serverName), _realName(realName)
{
    //este comentario es para probar
    this->_nickName = "niki";
}

User::~User()
{
}

std::string User::getUsername() const
{
    return this->_userName;
}

void User::setUsername(const std::string &username)
{
    this->_userName = username;
}

std::string User::getNickname() const
{
    return this->_nickName;
}

void User::setNickname(const std::string &nickname)
{
    this->_nickName = nickname;
}

std::string User::getOldNick() const
{
    return this->_oldNick;
}

void User::setOldNick(const std::string &oldNick)
{
    this->_oldNick = oldNick;
}

std::string User::getHostname() const
{
    return this->_hostName;
}

void User::setHostname(const std::string &hostname)
{
    this->_hostName = hostname;
}

int User::getFd() const
{
    return this->_fd;
}
