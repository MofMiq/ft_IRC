#include "../inc/Server.hpp"

//LOS CODIGOS DE ERROR SON REPLY DEL POLLOUT CON LA FUNCION SEND
//LOS MENSAJES SON MESSAGE DEL POLLOUT CON LA FUNCION SEND
//CUANDO LAS COMUNICACIONES SON DEL SERVER AL CLIENTE NO SE PONE EL FROM PORQUE ES DEL PROPIO SERVER Y EL TO ES EL FD DEL DESTINATARIO
//SI LA PASS ES OK RPL_WELCOME

Server* Server::instance = NULL;

Server::Server(int port, const std::string& password) : port(port), password(password), server_socket(-1), serverName("MyServer") {}

void Server::sendMessageClient(int clientSocket, const std::string& errorMsg) 
{
/*     std::cout << "MENSAJE RECIBIDO DEL SERVER" << std::endl;
    std::cout << PURPLE << errorMsg << std::endl;
    for(size_t i = 0; i < errorMsg.length(); i++) //borrar debug
    {
        std::cout << RED << static_cast<int>(errorMsg[i]) << " ";
    }
    std::cout << END << std::endl; */

    std::string message = errorMsg + "\r\n";
    send(clientSocket, message.c_str(), message.length(), 0);
}

bool    Server::start()
{
    Server::instance = this;
    std::signal(SIGINT, Server::signalHandler);

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

void    Server::checkQueue()
{
	for (std::map<int, User* >::iterator it = this->_usersServerByFd.begin(); it != this->_usersServerByFd.end(); ++it)
	{
		if (it->second->getStandBy() == false)
		{
			while (it->second->getQueueSize() > 0)
			{
				sendMessageClient(it->second->getFd(), it->second->dequeueResponse());
			}
		}
	}
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
				}
				else {
					handle_client_message(pollfds[i].fd);
				}
			}
		}
		checkQueue();
	}
}

std::string     Server::getServerName() const 
{
    return this->serverName;
}

void    Server::handle_new_connection() 
{
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
    pfd.events = POLLIN | POLLOUT;
    pfd.revents = 0;
    pollfds.push_back(pfd);
    clients[client_socket] = "";
    _usersServerByFd[client_socket] = new User(client_socket);
    //_usersServerByFd[client_socket] = User(client_socket);
    _usersServerByFd[client_socket]->setUsername("");
    _usersServerByFd[client_socket]->setNickname("");
    _usersServerByFd[client_socket]->setHostname("");
    _usersServerByFd[client_socket]->setServername("");
    _usersServerByFd[client_socket]->setRealname("");
    //this->_authenticated = false; SE GUARDABA EN EL SERVER Y NO EN EL USER
    _usersServerByFd[client_socket]->setAuthenticated(false);
    _usersServerByFd[client_socket]->setCapLS(false);
    //falta implementar la eliminación del usuario del mapa _usersServerByFd cuando se elimina el usuario del servidor.

    std::cout << "New connection accepted" << std::endl;
}

std::string     Server::extractPassword(std::string strRaw)
{
/*     std::cout << "EXTRACT PASSWORD" << std::endl;
    std::cout << PURPLE << strRaw << " length: " << strRaw.length() << std::endl;
    for(size_t i = 0; i < strRaw.length(); i++) //borrar debug
    {
        std::cout << RED << "[" << i << "]" << static_cast<int>(strRaw[i]) << " ";
    }
    std::cout << END << std::endl; */

    std::string pass = "PASS";
    size_t position = strRaw.find(pass);

    if (position == std::string::npos)
        return ("");

    position += pass.size();

    size_t start_word = strRaw.find_first_not_of(" \t\n\r", position);
    size_t end_word = strRaw.find_first_of(" \t\n\r", start_word);

    //std::cout << "position: " << position << " / start_word: " << start_word << " / end_word: " << end_word << std::endl; //borrar debug

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

    size_t start_word = strRaw.find_first_not_of(" \r\n", position);
    size_t end_word = strRaw.find_first_of(" \r\n", start_word);
    userName = strRaw.substr(start_word, end_word - start_word);

    start_word = strRaw.find_first_not_of(" \r\n", end_word);
    end_word = strRaw.find_first_of(" \r\n", start_word);
    hostName = strRaw.substr(start_word, end_word - start_word);

    start_word = strRaw.find_first_not_of(" \r\n", end_word);
    end_word = strRaw.find_first_of(" \r\n", start_word);
    serverName = strRaw.substr(start_word, end_word - start_word);

    start_word = strRaw.find_first_not_of(" \r\n", end_word);
    end_word = strRaw.find_first_of(" \r\n", start_word);
    realName = strRaw.substr(start_word, end_word - start_word);
}

std::string     Server:: extractInfo(std::string& strRaw, const std::string& cmd)
{
    std::string message = strRaw;
    size_t startPos = message.find(cmd);
    if (startPos != std::string::npos) {
        size_t endPos = message.find_first_of("\n", startPos);
        if (endPos != std::string::npos) {
            return message.substr(startPos, (endPos + 1) - startPos);
        }
    }
    return ""; 
}

void    Server::handle_client_message(int client_socket) 
{
    bool done = false;
    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            std::cout << "Client disconnected" << std::endl;
        }
        else {
            perror("read");
        }

        close(client_socket);
        remove_client(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    std::string message(buffer);


    std::cout << "MENSAJE RECIBIDO DEL CLIENTE" << std::endl;
    std::cout << message << " size: " << message.length() << std::endl;
/*    for(size_t i = 0; i < message.length(); i++) //borrar debug
    {
        std::cout << RED << static_cast<int>(message[i]) << " ";
    }
    std::cout << END << std::endl; */
    // std::cout << "VALOR CAPLS = " << _usersServerByFd[client_socket]->getCapLS() << std::endl;


    if (message.find("CAP LS") != std::string::npos)
        _usersServerByFd[client_socket]->setCapLS(true);

    if (message.find("PASS") == std::string::npos && this->_usersServerByFd[client_socket]->getAuthenticated() == false && _usersServerByFd[client_socket]->getCapLS() == false)
    {
        std::cout << "EL CLIENTE CON FD -> " << client_socket << " INTENTA EJECUTAR COMANDOS SIN AUTENTICARSE. EXPULSADO" << std::endl;
        sendMessageClient(client_socket, "EXPULSADO. DEBE AUTENTICARSE ANTES DE REALIZAR NINGUNA ACCIÓN");
        close(client_socket);
        remove_client(client_socket);
        return;
    }

    //anidar las condiciones?
    try
    {
        if (message.find("PASS") != std::string::npos && this->_usersServerByFd[client_socket]->getAuthenticated() == false)
        {
            done = true;
            std::string userPass = extractPassword(message);
            if (userPass != "")
            {
                // std::cout << "CONTRASEÑA GUARDADA = " << userPass << std::endl;
                // std::cout << "VALOR CAPLS = " << _usersServerByFd[client_socket]->getCapLS() << std::endl;
/*                 if (_usersServerByFd[client_socket]->getCapLS() == true)
                    userPass = userPass.substr(0, userPass.size() - 1); */
                // std::cout << "LONGITUD CONTRASEÑA INTRODUCIDA = " << userPass.size() << std::endl;
                if (userPass == this->password)
                {
                    std::cout << "CONTRASEÑA OK" << std::endl;
                    this->_usersServerByFd[client_socket]->setAuthenticated(true);
                }
                else
                {
                    std::cout << "CONTRASEÑA ERRONEA" << std::endl;
                    // //Se devuelve al cliente el mensaje ERR_PASSWDMISMATCH (464)
                    // sendMessageClient(client_socket, "<client> :Password incorrect");
                    // //Se envia el mensaje de ERROR al cliente antes de cerrar la conexion
                    // sendMessageClient(client_socket, "ERROR :Wrong password");
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
    }
    catch (...)
    {
        std::cerr << "ERROR EN PASS" << std::endl;
    }
    try
    {
        //esta implementación de NICK sirve sólo para la primera vez.
        if (message.find("NICK") != std::string::npos && this->_usersServerByFd[client_socket]->getNickname() == "") 
        {
            done = true;
            std::string nickName;

            nickName = extractInfo(message, "NICK");
            if (nickName != "")
            {
                std::cout << "NICKNAME EXTRAIDO = " << nickName << std::endl;
/*                 this->_usersServerByNick[nickName] = client_socket;
                this->_usersServerByFd[this->_usersServerByNick[nickName]]->setNickname(nickName);
                std::cout << "NICKNAME DEL CLIENTE = " << this->_usersServerByFd[_usersServerByNick[nickName]]->getNickname() << std::endl;
                clients[client_socket] = nickName; */
                processClientBuffer(client_socket, nickName);

            }
        }
    }
    catch(...)
    {
        std::cerr << "ERROR EN NICK" << std::endl;
    }
    try
    { 
        if (message.find("USER") != std::string::npos && this->_usersServerByFd[client_socket]->getUsername() == "")
        {
            done = true;
            std::string userCmd = extractInfo(message, "USER");
            if (userCmd != "")
            {
                std::cout << "USERNAME EXTRAIDO = " << userCmd << std::endl;
                processClientBuffer(client_socket, userCmd);

            }
/*             std::string userName, hostName, serverName, realName;
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
                        //condicion para que sea cuando realmente te unes
            sendMessageClient(client_socket, rplWelcome(*this, *this->_usersServerByFd[client_socket]));
            sendMessageClient(client_socket, rplYourHost(*this, *this->_usersServerByFd[client_socket]));
            sendMessageClient(client_socket, rplCreated(*this, *this->_usersServerByFd[client_socket]));
            sendMessageClient(client_socket, rplMyInfo(*this, *this->_usersServerByFd[client_socket])); */


            //Añadir usuario al contenedor de usuarios del server
            //_usersServer[fdCliente] = new User(todos los datos);
            //hacer una funcion con un mapa que transforme el NICK del usuario en su FD
            //  este NICK se agrega con el comando NICK que es cuando se crea la equivalencia NICK -> FD en el mapa que se guarda en Server
        }
    }
    catch(...)
    {
        std::cerr << "ERROR EN USER" << std::endl;
    }
    if (done == false)
        processClientBuffer(client_socket, message);
}

void    Server::printUSBN(std::map < std::string, int >& map)
{
    std::cout << "IMPRIMIENDO CONTENIDO DE USERS_BY_NICK" << std::endl;
    for (std::map < std::string, int >::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << "[" << it->first << "] -> " << it->second << std::endl;
    }
}

void    Server::printUSBF(std::map < int, User* >& map)
{
    std::cout << "IMPRIMIENDO CONTENIDO DE USERS_BY_FD" << std::endl;
    for (std::map < int, User* >::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << "[" << it->first << "] -> " << it->second << std::endl;
    }
}

void Server::remove_client(int client_socket) 
{
    for (std::vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it) {
        if (it->fd == client_socket) {
            pollfds.erase(it);
            break;
        }
    }

    // std::cout << "ESTADO DE LOS MAPAS ANTES DE ELIMINAR ELEMENTOS" << std::endl;
    // printUSBF(_usersServerByFd);
    // printUSBN(_usersServerByNick);

    // std::cout << "client_socket = " << client_socket << std::endl;
    // std::cout << "clients[client_socket] = " << clients[client_socket] << std::endl;

    if (_usersServerByNick.find(clients[client_socket]) != _usersServerByNick.end())
        _usersServerByNick.erase(clients[client_socket]);
    if (_usersServerByFd.find(client_socket) != _usersServerByFd.end())
    {
        delete _usersServerByFd[client_socket];
        _usersServerByFd.erase(client_socket);
    }
    clients.erase(client_socket);

    // std::cout << "ESTADO DE LOS MAPAS DESPUES DE ELIMINAR ELEMENTOS" << std::endl;
    // printUSBF(_usersServerByFd);
    // printUSBN(_usersServerByNick);
}


bool Server::channelExists(const std::string& channelName) 
{
    return _channelsServer.find(channelName) != _channelsServer.end();
}

void Server::addUserToChannel(User& user, const std::string& channelName) 
{
    if (!channelExists(channelName)) {
        _channelsServer[channelName] = Channel(channelName);
    }
    _channelsServer[channelName].addUser(&user);
    //user._channelIn.push_back(channelName); //struct response?
}

void Server::createChannel(const std::string& channelName, const std::string& key, const std::string& timestamp)
{
    //if (!channelExists(channelName))
    //    _channelsServer[channelName] = Channel(channelName);
    Channel newChannel(channelName);
    newChannel.setCreationTime(timestamp);
    if (!key.empty()) {
        newChannel.setPassNeeded(true);
        newChannel.setPass(key);
    }
    _channelsServer[channelName] = newChannel;
}


Channel* Server::getChannel(const std::string& channelName) 
{
    std::map<std::string, Channel>::iterator it = _channelsServer.find(channelName);
    if (it != _channelsServer.end()) {
        return &it->second; // Devuelve un puntero al canal si se encuentra
    }
    return NULL; // Devuelve NULL si el canal no se encuentra
}

bool Server::isUserInChannelServer(User& user, const std::string& channelName)
{
    return _channelsServer[channelName].isUserInChannel(user.getFd());
}

void Server::ShowChannelsAndUsers() const 
{
    std::cout << "List of channels and users:" << std::endl;
    for (std::map<std::string, Channel>::const_iterator it = _channelsServer.begin(); it != _channelsServer.end(); ++it)
    {
        const std::string& channelName = it->first;
        const Channel& channel = it->second;
        std::vector<User*> users = channel.getUsers();
        std::cout << "Channel: " << channelName << std::endl;
        for (std::vector<User*>::const_iterator userIt = users.begin(); userIt != users.end(); ++userIt)
        {
            User* user = *userIt;
            std::cout << " - User: " << user->getNickname() << " (FD: " << user->getFd() << ")"; 
            if(channel.isUserOperator(user->getFd()))
                std::cout <<" IS OPERATOR";
            std::cout << std::endl;
        }
    }
}

// Asigna el rol de operador a un usuario en un canal
void Server::setOperator(const User& user, const std::string& channelName) 
{
    _channelsServer[channelName].addOperator(user.getFd());
}

bool Server::checkChannelKey(const std::string& channelName, const std::string& key)
{
    // Obtener la referencia al canal
    Channel& channel = _channelsServer[channelName];
    //Si el canal no necesita clave devolver true directamente
    if (!channel.getPassNeeded())
        return true;
    // Verificar si la clave del canal coincide
    if (channel.getPass() == key)
        return true;
    //Si llega aqui no coincide, devuelve false
    return false;
}

bool Server::isNickInServer(const std::string& nick)
{
    if (nick == "bot") //borrar debug? esto es para que no cuente a bot en caso de que alguien quiera mandarle un mensaje
        return true;
    std::map<std::string, int>::const_iterator it = this->_usersServerByNick.find(nick);
    if (it != this->_usersServerByNick.end())
        return true;
    return false;
}

//void Server::updateUsersServerByNick(int fd, const std::string& newNick)
//{
//    for (std::map<std::string, int>::iterator it = this->_usersServerByNick.begin(); it != this->_usersServerByNick.end(); ++it)
//    {
//        if (it->second == fd)
//        {
//            this->_usersServerByNick.erase(it);
//            this->_usersServerByNick[newNick] = fd;
//        }
//    }
//}

void Server::updateUsersServerByNick(int fd, const std::string& newNick)
{
    std::map<std::string, int>::iterator it;

    // Buscamos el iterador del elemento con el fd dado
    for (it = this->_usersServerByNick.begin(); it != this->_usersServerByNick.end(); ++it)
    {
        if (it->second == fd)
        {
            break;
        }
    }

    // Si encontramos el elemento, actualizamos el mapa
    if (it != this->_usersServerByNick.end())
    {
        this->_usersServerByNick.erase(it);  // Borramos el elemento antiguo
        this->_usersServerByNick[newNick] = fd;  // Insertamos el nuevo elemento
    }
}



//Cambiado pq da segfault si no existia el user, hay q recorrer antes y ver si existe
User* Server::getUserByNick(const std::string& nick)
{
    std::map<std::string, int>::iterator it = _usersServerByNick.find(nick);
    if (it == _usersServerByNick.end()) {
        return NULL;  // Nick no encontrado
    }
    int fd = it->second;
    std::map<int, User*>::iterator userIt = _usersServerByFd.find(fd);
    if (userIt == _usersServerByFd.end()) {
        return NULL;  // FD no encontrado
    }
    return userIt->second;
}

std::vector<Channel*> Server::getAllChannelsUserIn(int fd)
{
	std::vector<Channel*> channelsUserIn;
	for (std::map<std::string, Channel>::iterator it = this->_channelsServer.begin(); it != this->_channelsServer.end(); ++it)
	{
		if (it->second.isUserInChannel(fd))
			channelsUserIn.push_back(&(it->second));
	}
	return channelsUserIn;
}

void        Server::signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        Server::instance->cleanAll();
        exit(EXIT_SUCCESS);
    }
}

void        Server::cleanAll()
{
    for (std::map < int, User* >::iterator it = this->_usersServerByFd.begin(); it != this->_usersServerByFd.end(); it++)
    {
        close (it->first);
        delete it->second;
    }

    _usersServerByFd.clear();

    if (server_socket != -1)
    {
        close(server_socket);
        server_socket = -1;
    }

    pollfds.clear();

    _usersServerByNick.clear();
    _channelsServer.clear();
    clients.clear();
    _clientBuffers.clear();

    std::cout << "* ALL CLEAN *" << std::endl;
}

void Server::processClientBuffer(int client_socket, const std::string& message_fragment)
{ 
    this->_clientBuffers[client_socket] += message_fragment;

    size_t pos;
    while ((pos = this->_clientBuffers[client_socket].find('\n')) != std::string::npos)
    {
        std::string command = this->_clientBuffers[client_socket].substr(0, pos);
        Command cmd(command);
        User* user = this->_usersServerByFd[client_socket];
        cmd.parseCommand(cmd.getArg(0), this, *user);
        this->_clientBuffers[client_socket].erase(0, pos + 1);
    }
}

std::string&    Server::getPass()
{
    return this->password;
}
