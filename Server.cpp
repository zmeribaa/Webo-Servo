#include "Server.hpp"

Server::Server()
{
    //std::cout << "We've got a server" << std::endl;
}


void Server::debug()
{
    std::cout << std::endl << "Server " << _name << " config:" << std::endl; 

    std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    // Iterate over the map using iterator
    while(it != keys.end())
    {
        std::cout << it->first << " :: " << it->second<<std::endl;
        it++;
    }

    std::vector<Location>::iterator ptr;
    for (ptr = locations.begin(); ptr != locations.end(); ++ptr)
        ptr->debug();

    /*std::cout << "Locations config:" << std::endl; 
    for (auto it = begin(locations); it != end(locations); ++it)
        it->debug();*/
}

bool Server::attach(std::string pair)
{
    std::vector<std::string> pairs = split(pair, ':');
    if (!pairs.empty())
        keys[pairs[0]] = pairs[1];
    else
       std::cout << "This shit so empty" << std::endl;
    return 1;
}

void Server::setName(std::string name)
{
    _name = name; 
}


void Server::attach(const Location location)
{
    locations.push_back(location);
}