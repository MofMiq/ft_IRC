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
# include "Commands.hpp"
# include "User.hpp"
# include "Channel.hpp"

const int MAX_CLIENTS = 100;
const int BUFFER_SIZE = 1024;

class Server {
private:
    int port;
    std::string password;
    int server_socket;
    std::string serverName;
    std::vector<struct pollfd> pollfds;
    std::map<int, std::string> clients; // Map from socket to client nickname SE UTILIZA PARA EL POLL
    std::map<std::string, Channel* > _channelsServer; // Map de NOMBRE CANAL y puntero a CHANNEL correspondiente
    std::map<std::string, User* > _usersServer; // Map de NICK y puntero a USER correspondiente

public:
    Server(int port, const std::string& password);
    bool start();
    void run();
    std::string getServerName() const;
    void handle_new_connection();
    std::string extractPassword(std::string strRaw);
    void handle_client_message(int client_socket);
    void remove_client(int client_socket);
};

#endif