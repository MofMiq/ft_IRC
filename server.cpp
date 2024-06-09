#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <fcntl.h>
#include <algorithm>

const int MAX_CLIENTS = 100;
const int BUFFER_SIZE = 1024;

class IRCServer {
public:
    IRCServer(int port, const std::string& password)
        : port(port), password(password), server_socket(-1) {}

    bool start() {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            perror("socket");
            return false;
        }

        // Configurar el socket como no bloqueante
        int flags = fcntl(server_socket, F_GETFL, 0);
        if (flags == -1 || fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl");
            close(server_socket);
            return false;
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("bind");
            close(server_socket);
            return false;
        }

        if (listen(server_socket, MAX_CLIENTS) == -1) {
            perror("listen");
            close(server_socket);
            return false;
        }

        struct pollfd pfd;
        pfd.fd = server_socket;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollfds.push_back(pfd);

        std::cout << "Server started on port " << port << std::endl;
        return true;
    }

    void run() {
        while (true) {
            int poll_count = poll(pollfds.data(), pollfds.size(), -1);
            if (poll_count == -1) {
                perror("poll");
                break;
            }

            for (size_t i = 0; i < pollfds.size(); ++i) {
                if (pollfds[i].revents & POLLIN) {
                    if (pollfds[i].fd == server_socket) {
                        handle_new_connection();
                    } else {
                        handle_client_message(pollfds[i].fd);
                    }
                }
            }
        }
    }

private:
    int port;
    std::string password;
    int server_socket;
    std::vector<struct pollfd> pollfds;
    std::map<int, std::string> clients; // Map from socket to client nickname
    std::map<std::string, std::set<int> > channels; // Map from channel name to set of clients

    void handle_new_connection() {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket == -1) {
            perror("accept");
            return;
        }

        // Configurar el socket del cliente como no bloqueante
        int flags = fcntl(client_socket, F_GETFL, 0);
        if (flags == -1 || fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl");
            close(client_socket);
            return;
        }

        struct pollfd pfd;
        pfd.fd = client_socket;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollfds.push_back(pfd);
        clients[client_socket] = "";

        std::cout << "New connection accepted" << std::endl;
    }

    void handle_client_message(int client_socket) {
        char buffer[BUFFER_SIZE];
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                std::cout << "Client disconnected" << std::endl;
            } else {
                perror("read");
            }

            close(client_socket);
            remove_client(client_socket);
            return;
        }

        buffer[bytes_read] = '\0';
        std::string message(buffer);

        if (message.substr(0, 5) == "NICK ") {
            clients[client_socket] = message.substr(5);
        } else if (message.substr(0, 5) == "JOIN ") {
            std::string channel = message.substr(5);
            channels[channel].insert(client_socket);
        } else if (message.substr(0, 5) == "PART ") {
            std::string channel = message.substr(5);
            channels[channel].erase(client_socket);
            if (channels[channel].empty()) {
                channels.erase(channel);
            }
        } else if (message.substr(0, 5) == "KICK ") {
            // Handle KICK command
        } else if (message.substr(0, 7) == "INVITE ") {
            // Handle INVITE command
        } else if (message.substr(0, 6) == "TOPIC ") {
            // Handle TOPIC command
        } else if (message.substr(0, 5) == "MODE ") {
            // Handle MODE command
        } else {
            // Broadcast message to all clients in the channel
            std::map<std::string, std::set<int> >::iterator it;
            for (it = channels.begin(); it != channels.end(); ++it) {
                if (it->second.find(client_socket) != it->second.end()) {
                    std::set<int>::iterator client_it;
                    for (client_it = it->second.begin(); client_it != it->second.end(); ++client_it) {
                        if (*client_it != client_socket) {
                            send(*client_it, message.c_str(), message.size(), 0);
                        }
                    }
                }
            }
        }
    }

    void remove_client(int client_socket) {
        std::vector<struct pollfd>::iterator it;
        for (it = pollfds.begin(); it != pollfds.end(); ++it) {
            if (it->fd == client_socket) {
                pollfds.erase(it);
                break;
            }
        }
        clients.erase(client_socket);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string password = argv[2];

    IRCServer server(port, password);
    if (!server.start()) {
        return 1;
    }

    server.run();
    return 0;
}
