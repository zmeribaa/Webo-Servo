#include "Config.hpp"
#include "Location.hpp"
#include "Server.hpp"
// Server belongs to Webserv
// Locations belong to Server

// Terminate on these errors
//      - Unvalid key
//      - Could not open config file (Both user config or default config)


// Show warning in these errors
//      - Missing important values and print default

void createServer()
{
    // Will create and return a server class
    // With all the values, parsed
    // Attach all parsed locations to server
    // Attach server to Webserve
}

void Config::attach(const Server server)
{
    servers.push_back(server);
}

void Config::debug()
{
    std::cout << "List of parsed servers and their config" << std::endl;
    std::cout << "__________________________________________________" << std::endl;

    std::vector<Server>::iterator ptr;
    for (ptr = servers.begin(); ptr != servers.end(); ptr++)
        ptr->debug();
}

void Config::save_location(Server *server, std::istream &config_file_stream, std::string line)
{
    Location *location = new Location();
    location->setPath(line);
    int         pos;

    while (std::getline(config_file_stream, line))
    {
        if ((pos = line.find("{")) != std::string::npos && line.find("}", pos + 1) != std::string::npos)
        {
            server->attach(*location);
            delete location;
            save_location(server, config_file_stream, line);
            break ;
        }
        else if ((pos = line.find("[")) != std::string::npos && line.find("]", pos + 1) != std::string::npos)
        {
            server->attach(*location);
            delete location;
            attach(*server);
            delete server;
            save_server(config_file_stream, line);
        }
        else
        {
            location->attach(line);
        }
    }

}

void Config::save_server(std::istream &config_file_stream, std::string line)
{
    Server *server = new Server();
    server->setName(line);
    int         pos;
    while (std::getline(config_file_stream, line))
    {
        if ((pos = line.find("{")) != std::string::npos && line.find("}", pos + 1) != std::string::npos)
        {
            save_location(server, config_file_stream, line);
        }
        else if ((pos = line.find("[")) != std::string::npos && line.find("]", pos + 1) != std::string::npos)
        {
            attach(*server);
            delete server ;
            save_server(config_file_stream, line);
        }
        else
        {
            server->attach(line);
            if (config_file_stream.eof())
                attach(*server);
        }
    }
}

Config::Config(std::string config_file): config_file(config_file)
{
    std::ifstream config_file_stream(config_file);
    if (!config_file_stream.is_open())
        throw std::runtime_error("Could not open config file");
    std::string line;
    std::string line_tmp;
    int         pos;

    while (std::getline(config_file_stream, line))
    {
        if ((pos = line.find("[")) != std::string::npos && line.find("]", pos + 1) != std::string::npos)
            save_server(config_file_stream, line);
    }
}

Config::~Config()
{

}