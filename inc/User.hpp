#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <queue>
//#include "Server.hpp"
#define MAX_LENGHT 10
class User
{
private:
    //USER <usuario> <host> <servidor> <nombre_real>
    int _fd; // File descriptor para el poll
    bool        _authenticated;
    bool        _commandUSER;
    bool        _commandNICK;
    bool        _configOK;
    bool        _registered;
    bool        _capLS;
    bool        _flag;
    std::string _userName;
    std::string _nickName;
    std::string _oldNick;
    std::string _hostName;
    std::string _serverName;
    std::string _realName;
    std::queue<std::string> _responses;
    std::vector<std::string> _invitedChannels;

public:
    User(int fd);
    ~User();

    // Getters y setters
    bool getCapLS() const;
    void setCapLS(bool value);

    bool getAuthenticated() const;
    void setAuthenticated(bool value);

    bool getCommandUSER() const;
    void setCommandUSER(bool value);

    bool getCommandNICK() const;
    void setCommandNICK(bool value);

    bool getConfigOK() const;
    void setConfigOK(bool value);

    bool getRegistered() const;
    void setRegistered(bool value);

    bool getFlag() const;
    void setFlag(bool value);

    std::string getUsername() const;
    void setUsername(const std::string& username); //Pa el comando USER

    std::string getNickname() const;
    void setNickname(const std::string& nickname); //PA el comando NICK

    std::string getOldNick() const;
    void setOldNick(const std::string& oldNick); //PA el comando NICK

    std::string getHostname() const;
    void setHostname(const std::string& hostname);

    std::string getServername() const;
    void setServername(const std::string& servername);

    std::string getRealname() const;
    void setRealname(const std::string& realname);

    //Para join, si esta invitado al canal
    void addInvitedChannel(const std::string& channelName);
    bool isInvitedToChannel(const std::string& channelName) const;

    int getFd() const;
    bool getStandBy();
    void setStandBy(bool cond);
    void setPass(int pass);

    bool checkResponsesQueue();
    int     getQueueSize();
    void enqueueResponse(const std::string& res);   //meter un msg
    std::string dequeueResponse();                  //sacar un msg
};

#endif