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
# include <csignal>
# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Response.hpp"

const int MAX_CLIENTS = 100;
const int BUFFER_SIZE = 1024;

class Server {
private:
    static Server* instance;
    //bool _authenticated;
    int port;
    std::string password;
    int server_socket;
    std::string serverName;
    std::vector<struct pollfd> pollfds;
    std::map<int, std::string> clients; // Map from socket to client nickname SE UTILIZA PARA EL POLL
    std::map<std::string, Channel> _channelsServer;
 //   std::map<std::string, User> _nickToUser; // Mapa para acceder a los usuarios por nickname TODO
    std::map<int, std::string> _clientBuffers;

public:
    std::map<int, User* > _usersServerByFd; // Map de FD y puntero a USER correspondiente
    Server(int port, const std::string& password);
    void        sendMessageClient(int clientSocket, const std::string& errorMsg);
    bool        start();
    void        checkQueue();
    void        run();
    std::string getServerName() const;
    void        handle_new_connection();
    std::string extractPassword(std::string strRaw);
    void        handle_client_message(int client_socket);
    void        remove_client(int client_socket);

    bool        channelExists(const std::string& channelName);
    void        addUserToChannel(User& user, const std::string& channelName);
    void        createChannel(const std::string& channelName, const std::string& key, const std::string& timestamp);
    Channel*    getChannel(const std::string& channelName);
    bool        isUserInChannelServer(User& user, const std::string& channelName);
    void        ShowChannelsAndUsers() const;
    void        setOperator(const User& user, const std::string& channelName);
    bool        checkChannelKey(const std::string& channelName, const std::string& key);
    std::string&    getPass();
    std::string     extractInfo(std::string& strRaw, const std::string& cmd);
    
    void processClientBuffer(int client_socket, std::string message_fragment);
    void addUserToNickMap(const std::string& nick, int fd);

    User*       getUserByNick(const std::string& nick);   // Metodo para obtener un usuario por su nickname

    //Commands functions
    bool        isNickInServer(const std::string& nick);
    void        updateUsersServerByNick(int fd, const std::string& newNick);
    std::vector<Channel*> getAllChannelsUserIn(int fd);

    //FUNCIONES PARA DEPURACION
    void        printUSBN(std::map < std::string, int >& map);
    void        printUSBF(std::map < int, User* >& map);

    //FUNCION DE LIMPIEZA AL CERRAR
    void        cleanAll();

    //FUNCION CONTROL SEÑAL SALIDA
    static void        signalHandler(int signal);
};

#endif