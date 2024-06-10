#include "../inc/User.hpp"

User::User(int fd) : _fd(fd)
{
    //esto es solo para poder iniciar la clase
    this->_userName = "LA";
    this->_nickName = "PO";
    this->_hostName = "RU";
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
