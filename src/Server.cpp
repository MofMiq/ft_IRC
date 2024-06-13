#include "../inc/Server.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password), server_socket(-1), serverName("MyServer") {}

bool    Server::start()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return false;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_socket);
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
    _usersServerByFd[client_socket] = new User(client_socket);
    _usersServerByFd[client_socket]->setUsername("");
    _usersServerByFd[client_socket]->setNickname("");
    _usersServerByFd[client_socket]->setHostname("");
    _usersServerByFd[client_socket]->setServername("");
    _usersServerByFd[client_socket]->setRealname("");
    this->_authenticated = false;
    //falta implementar la eliminación del usuario del mapa _usersServerByFd cuando se elimina el usuario del servidor

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

void        Server::extractDataUser(std::string strRaw, std::string& userName, std::string& hostName, std::string& serverName, std::string& realName)
{
    //USER <usuario> <host> <servidor> <nombre_real>

    std::string user = "USER";
    size_t position = strRaw.find(user);

    if (position == std::string::npos)
        return;

    position += user.size();

    size_t start_word = strRaw.find_first_not_of(" \t\n", position);
    size_t end_word = strRaw.find_first_of(" \t\n", start_word);
    userName = strRaw.substr(start_word, end_word - start_word);
    
    start_word = strRaw.find_first_not_of(" \t\n", end_word);
    end_word = strRaw.find_first_of(" \t\n", start_word);
    hostName = strRaw.substr(start_word, end_word - start_word);

    start_word = strRaw.find_first_not_of(" \t\n", end_word);
    end_word = strRaw.find_first_of(" \t\n", start_word);
    serverName = strRaw.substr(start_word, end_word - start_word);

    start_word = strRaw.find_first_not_of(" \t\n", end_word);
    end_word = strRaw.find_first_of(" \t\n", start_word);
    realName = strRaw.substr(start_word, end_word - start_word);
}

std::string     Server::extractNick(std::string strRaw)
{
    std::string nick = "NICK";
    size_t position = strRaw.find(nick);

    if (position == std::string::npos)
        return ("");

    position += nick.size();

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

    if (message.find("PASS") == std::string::npos && this->_authenticated == false)
    {
        std::cout << "EL CLIENTE CON FD -> " << client_socket << " INTENTA EJECUTAR COMANDOS SIN AUTENTICARSE. EXPULSADO" <<std::endl;
        close(client_socket);
        remove_client(client_socket);
        return;
    }

    if (message.find("PASS") != std::string::npos && this->_authenticated == false)
    {
        std::string userPass = extractPassword(message);
        if (userPass != "")
        {
            std::cout << "CONTRASEÑA GUARDADA = " << userPass << std::endl;
            if (userPass == this->password)
            {
                std::cout << "CONTRASEÑA OK" << std::endl;
                this->_authenticated = true;
            }
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

    if (message.find("USER") != std::string::npos && this->_usersServerByFd[client_socket]->getUsername() == "")
    {
        std::string userName, hostName, serverName, realName;
        extractDataUser(message, userName, hostName, serverName, realName);
        this->_usersServerByFd[client_socket]->setUsername(userName);
        this->_usersServerByFd[client_socket]->setHostname(hostName);
        this->_usersServerByFd[client_socket]->setServername(serverName);
        this->_usersServerByFd[client_socket]->setRealname(realName);

        std::cout << "DATOS COMPLETOS DEL USUARIO CON FD -> " << this->_usersServerByFd[client_socket]->getFd() << std::endl;
        std::cout << "USERNAME -> " << this->_usersServerByFd[client_socket]->getUsername() << std::endl;
        std::cout << "HOSTNAME -> " << this->_usersServerByFd[client_socket]->getHostname() << std::endl;
        std::cout << "SERVERNAME -> " << this->_usersServerByFd[client_socket]->getServername() << std::endl;
        std::cout << "REALNAME -> " << this->_usersServerByFd[client_socket]->getRealname() << std::endl;

        //Añadir usuario al contenedor de usuarios del server
        //_usersServer[fdCliente] = new User(todos los datos);
        //hacer una funcion con un mapa que transforme el NICK del usuario en su FD
        //  este NICK se agrega con el comando NICK que es cuando se crea la equivalencia NICK -> FD en el mapa que se guarda en Server
    }

    //esta implementación de NICK sirve sólo para la primera vez
    if (message.find("NICK") != std::string::npos && this->_usersServerByFd[client_socket]->getNickname() == "") 
    {
        std::string nickName;

        nickName = extractNick(message);
        if (nickName != "")
        {
            std::cout << "NICKNAME EXTRAIDO = " << nickName << std::endl;
            this->_usersServerByNick[nickName] = client_socket;
            this->_usersServerByFd[this->_usersServerByNick[nickName]]->setNickname(nickName);
            std::cout << "NICKNAME DEL CLIENTE = " << this->_usersServerByFd[_usersServerByNick[nickName]]->getNickname() << std::endl;
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


bool Server::channelExists(const std::string& channelName) {
    return _channelsServer.find(channelName) != _channelsServer.end();
}

void Server::addUserToChannel(User& user, const std::string& channelName) {
    if (!channelExists(channelName)) {
        _channelsServer[channelName] = Channel(channelName);
    }
    _channelsServer[channelName].addUser(user);
}

void Server::createChannel(const std::string& channelName) {
    if (!channelExists(channelName)) {
        _channelsServer[channelName] = Channel(channelName);
    }
}
