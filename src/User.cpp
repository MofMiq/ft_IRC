#include "../inc/User.hpp"
#include <stdexcept>
#include <iostream>

User::User(int fd) : 
_fd(fd),
_authenticated(false), 
_commandUSER(false), 
_commandNICK(false),
_configOK(false),
_registered(false),
_flag(false)
{
}

User::~User()
{
}

std::string User::getUsername() const
{
    return this->_userName;
}

void User::setUsername(const std::string& username)
{
    this->_userName = username;
}

std::string User::getNickname() const
{
    return this->_nickName;
}

void User::setNickname(const std::string& nickname)
{
    this->_nickName = nickname;
}

std::string User::getOldNick() const
{
    return this->_oldNick;
}

void User::setOldNick(const std::string& oldNick)
{
    this->_oldNick = oldNick;
}


std::string User::getHostname() const
{
    return this->_hostName;
}

void User::setHostname(const std::string& hostname)
{
    this->_hostName = hostname;
}

int User::getFd() const
{
    return this->_fd;
}

bool User::checkResponsesQueue()
{
    return this->_responses.empty();
}

void User::enqueueResponse(const std::string& res)
{
    this->_responses.push(res);
}

std::string User::dequeueResponse()
{
    try
    {
        if (this->_responses.empty())
            throw std::out_of_range("Queue is empty");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::string first = this->_responses.front();
    this->_responses.pop();
    return first;
}

std::string User::getServername() const
{
    return (this->_serverName);
}

void        User::setServername(const std::string& servername)
{
    this->_serverName = servername;
}

std::string User::getRealname() const
{
    return (this->_realName);
}

void        User::setRealname(const std::string& realname)
{
    this->_realName = realname;
}

int     User::getQueueSize()
{
    return (this->_responses.size());
}

void User::addInvitedChannel(const std::string& channelName)
{
    _invitedChannels.push_back(channelName);
}

bool User::isInvitedToChannel(const std::string& channelName) const
{
    for (std::vector<std::string>::const_iterator it = _invitedChannels.begin(); it != _invitedChannels.end(); ++it)
    {
        if (*it == channelName)
            return true;
    }
    return false;
}

bool User::getAuthenticated() const
{
    return (this->_authenticated);
}

void User::setAuthenticated(bool value)
{
    this->_authenticated = value;
}

bool User::getCommandUSER() const
{
    return (this->_commandUSER);
}

void User::setCommandUSER(bool value)
{
    this->_commandUSER = value;
}

bool User::getCommandNICK() const
{
    return (this->_commandNICK);
}

void User::setCommandNICK(bool value)
{
    this->_commandNICK = value;
}

bool User::getConfigOK() const
{
    return (this->_configOK);
}

void User::setConfigOK(bool value)
{
    this->_configOK = value;
}

bool User::getRegistered() const
{
    return (this->_registered);
}

void User::setRegistered(bool value)
{
    this->_registered = value;
}

bool User::getFlag() const
{
    return (this->_flag);
}

void User::setFlag(bool value)
{
    this->_flag = value;
}

bool User::getCapLS() const
{
    return (this->_capLS);
}

void User::setCapLS(bool value)
{
    this->_capLS = value;
}