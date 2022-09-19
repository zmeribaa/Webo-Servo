#include "Server.hpp"
#include "Request.hpp"

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

void Server::lessgo()
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	int opt = 1;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 54623\n\n Hello";
	
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(keys["server_name"].c_str());
    address.sin_port = htons(std::stoi(keys["listen"]));
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("Sockopt error");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("Sockopt error");
		exit(EXIT_FAILURE);
	}
			 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = recv( new_socket , buffer, 30000,0);

        std::string rt(buffer);
        Request request(rt);

        send(new_socket , hello.c_str() , hello.length(),0);

        close(new_socket);
    }
}

void Server::setName(std::string name)
{
    _name = name; 
}


void Server::attach(const Location location)
{
    locations.push_back(location);
}