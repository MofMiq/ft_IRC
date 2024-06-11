#include "../inc/Server.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password), server_socket(-1), serverName("MyServer") {}

bool    Server::start()
{
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

void    Server::run()
{
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

std::string     Server::getServerName() const {
    return this->serverName;
}