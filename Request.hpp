#ifndef REQUEST_HPP
# define REQUEST_HPP


#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Location.hpp"

// Create Request class with all nginx configuration parameters

class Request {
    public:
        Request();
        Request(std::string line);
        void debug();
        //~Request();
    private:
        std::unordered_map<std::string, std::string> keys;
};

#endif