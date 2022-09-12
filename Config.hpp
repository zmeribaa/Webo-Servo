// Config class with pointer to servers default constructor and deconsructor
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Server.hpp"

#include <iostream>
#include <fstream>

class Config {
    public:
        Config();
        Config(std::string config_file);
        ~Config();
        void debug();
        void attach(const Server server);
        void save_location(Server *server, std::istream &config_file_stream, std::string line);
        void save_server(std::istream &config_file_stream, std::string line);

    private:
        std::string config_file;
        std::vector<Server> servers;
};

#endif