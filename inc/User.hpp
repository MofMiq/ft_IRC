#ifndef USER_HPP
#define USER_HPP

#include <string>
//#include "Server.hpp"
class User
{
private:
    std::string _userName;
    std::string _nickName;
    std::string _hostName;
    int _fd; // File descriptor para el poll

public:
    User(int fd);
    ~User();

    // Getters y setters
    std::string getUsername() const;
    void setUsername(const std::string& username); //Pa el comando USER

    std::string getNickname() const;
    void setNickname(const std::string& nickname); //PA el comando NICK

    std::string getHostname() const;
    void setHostname(const std::string& hostname);

    int getFd() const;
};

#endif