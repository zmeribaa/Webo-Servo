#ifndef SERVER_HPP
# define SERVER_HPP

#include "Location.hpp"

// Create Server class with all nginx configuration parameters

class Server {
    public:
        Server();
        //~Server();
        void attach(const Location location);
        bool attach(std::string pair);
        void debug();
        void setName(std::string name);
    private:
        std::unordered_map<std::string, std::string> keys;
        std::string _name;
        std::vector<Location> locations;
};

#endif