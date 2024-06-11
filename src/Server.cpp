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

void    Server::handle_new_connection() {
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

std::string     Server::extractPassword(std::string strRaw)
{
    std::string pass = "PASS";
    size_t position = strRaw.find(pass);

    if (position == std::string::npos)
        return ("");

    position += pass.size();

    size_t start_word = strRaw.find_first_not_of(" \t\n", position);
    size_t end_word = strRaw.find_first_of(" \t\n", start_word);

    if (start_word != std::string::npos && end_word != std::string::npos)
        return (strRaw.substr(start_word, end_word - start_word));

    return ("");
}

void    Server::handle_client_message(int client_socket) {
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

    std::cout << "MENSAJE RECIBIDO DEL CLIENTE" << std::endl;
    std::cout << message << std::endl;

    if (message.find("PASS") != std::string::npos)
    {
        std::string userPass = extractPassword(message);
        if (userPass != "")
        {
            userPass = userPass.substr(0, userPass.size() - 1);
            if (userPass == this->password)
                std::cout << "CONTRASEÑA OK" << std::endl;
                //Añadir usuario al contenedor de usuarios del server
                //User u1(fd / NICK / todos los datos);
                //_usersServer[NICK] = u1.clone();
            else
            {
                std::cout << "CONTRASEÑA ERRONEA" << std::endl;
                close(client_socket);
                remove_client(client_socket);
                return;
            }
        }
        else
        {
            close(client_socket);
            remove_client(client_socket);
            return;
        }  
    }

    //DELETE -> de forma bruta voy a crear un User para poder compilar y probar algo
    User* usr1 = new User(client_socket); //leaks logicamente

    Command cmd(message);
    cmd.parseCommand(cmd.getArg(0), this, usr1);
    //para poder enviar la respuesta al cliente se usaría algo así
    //enviarRespuestaCliente(cmd.parseCommand(cmd.getArg(0), this, usr1));
}

void Server::remove_client(int client_socket) {
    std::vector<struct pollfd>::iterator it;
    for (it = pollfds.begin(); it != pollfds.end(); ++it) {
        if (it->fd == client_socket) {
            pollfds.erase(it);
            break;
        }
    }
    clients.erase(client_socket);
}