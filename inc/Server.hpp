#ifndef SERVER_HPP
# define SERVER_HPP
# include <stdio.h> //perror()
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <poll.h>
# include <cstring>
# include <fcntl.h>
# include <algorithm>
# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Response.hpp"

const int MAX_CLIENTS = 100;
const int BUFFER_SIZE = 1024;

class Server {
private:
    bool _authenticated;
    int port;
    std::string password;
    int server_socket;
    std::string serverName;
    std::vector<struct pollfd> pollfds;
    std::map<int, std::string> clients; // Map from socket to client nickname SE UTILIZA PARA EL POLL
    //std::map<std::string, Channel* > _channelsServer; // Map de NOMBRE CANAL y puntero a CHANNEL correspondiente
    std::map<std::string, int > _usersServerByNick; // Map de NICK a FD
    // std::map<int, User* > _usersServerByFd; // Map de FD y puntero a USER correspondiente
    std::map<int, User > _usersServerByFd; // Map de FD y puntero a USER correspondiente
    std::map<std::string, Channel> _channelsServer;

public:
    Server(int port, const std::string& password);
    void        sendMessageClient(int clientSocket, const std::string& errorMsg);
    bool        start();
    void        run();
    std::string getServerName() const;
    void        handle_new_connection();
    std::string extractPassword(std::string strRaw);
    std::string extractNick(std::string strRaw);
    void        extractDataUser(std::string strRaw, std::string& userName, std::string& hostName, std::string& serverName, std::string& realName);
    void        handle_client_message(int client_socket);
    void        remove_client(int client_socket);

    bool channelExists(const std::string& channelName);
    void addUserToChannel(User& user, const std::string& channelName);
    void createChannel(const std::string& channelName);

    //Commands functions
    bool isNickInServer(const std::string& nick);
    void updateUsersServerByNick(int fd, const std::string& newNick);
};

#endif