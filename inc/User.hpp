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
    std::string _userName;
    std::string _nickName;
    std::string _oldNick;
    std::string _hostName;
    std::string _serverName;
    std::string _realName;
    std::queue<std::string> _responses;
    std::vector<std::string> _invitedChannels; //Canales a los que el usuario ha sido invitado

    bool _standBy;

public:
    User();
    User(int fd);
    User(int fd, std::string userName, std::string hostName, std::string serverName, std::string realName);

    ~User();

    // Getters y setters
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

    bool checkResponsesQueue();
    //void enqueueResponse(Compose& cmp);   //meter un msg
    int     getQueueSize();
    void enqueueResponse(const std::string& res);   //meter un msg
    //Compose& dequeueResponse();                  //sacar un msg
    std::string dequeueResponse();                  //sacar un msg
};

#endif