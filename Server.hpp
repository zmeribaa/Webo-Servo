#ifndef SERVER_HPP
# define SERVER_HPP


#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 9000

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Location.hpp"

// Create Server class with all nginx configuration parameters

class Server {
    public:
        Server();
        //~Server();
        void attach(const Location location);
        bool attach(std::string pair);
        void debug();
        void lessgo();
        void setName(std::string name);
    private:
        std::unordered_map<std::string, std::string> keys;
        std::string _name;
        std::vector<Location> locations;
};

#endif