#pragma once

#include "Server.hpp"

class User
{
    public:
        std::string _username;
        std::string _nickname;
        std::string _hostname;
        int fd; // -> para el poll
        //contenedor para canal????

        //constructores&Destructores
        //getter&setter -> NICK & USER
};
