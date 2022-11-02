#include "Webserv.hpp"
#include "../Location/Location.hpp"
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

/*


TODO CHANGE ALL MAPS TO MULTIMAPS


*/



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

void Webserv::attach(const Server server)
{
    servers.push_back(server);
}

void Webserv::debug()
{

    std::cout << "List of parsed servers and their config" << std::endl;
    std::cout << "__________________________________________________" << std::endl;

    std::vector<Server>::iterator ptr;
    for (ptr = servers.begin(); ptr != servers.end(); ptr++)
    {
        ptr->debug();
    }

    std::cout << "First server test" << std::endl;
    std::cout << "__________________________________________________" << std::endl;
    
}

void Webserv::save_location(Server *server, std::istream &config_file_stream, std::string line)
{
    Location *location = new Location();
    line.erase(0, 1);
    line.resize(line.size() - 1);

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

void Webserv::save_server(std::istream &config_file_stream, std::string line)
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

Webserv::Webserv(std::string config_file): config_file(config_file)
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
    for (int i = 0; i < servers.size(); i++)
    {
        std::cout << "Creating socker for server " << i << std::endl;
        servers[i]._socket();
    }
    run();
}

Webserv::~Webserv()
{

}


bool	Webserv::is_serverfd(int fd)
{
	for (int i = 0; i < servers.size(); i++)
	{
		if (servers[i].getServerFd() == fd)
			return (true);
	}
	return (false);
}

int	Webserv::getindexConnex(int fd)
{
	for (int i = 0; i < servers.size(); i++)
	{
		if (servers[i].getConnexFd(fd) != -1)
			return (i);
	}
	return (-1);
}

int	Webserv::getindex(int fd)
{
	for (int i = 0; i < servers.size(); i++)
	{
		if (servers[i].getServerFd() == fd)
			return (i);
	}
	return (-1);
}


#define PRINTN(x) std::cout << x << std::endl;

void Webserv::run()
{
	int new_fd;
	int ret;
	int close_conn = FALSE;
	struct fd_set wset_master;
	struct fd_set rset_master;
	struct fd_set wset_working;
	struct fd_set rset_working;
	FD_ZERO(&wset_master);
	FD_ZERO(&rset_master);
    int connexionIndex;
	for (int i = 0; i < servers.size(); i++)
	{
		FD_SET(servers[i].getServerFd(), &rset_master);
        //FD_SET(servers[i].getServerFd(), &wset_master);
	}
	while (1)
	{
		rset_working = rset_master;
		wset_working = wset_master;
		ret = select(FD_SETSIZE, &rset_working, &wset_working, NULL, NULL);
		if (ret < 0)
		{
			std::cout << "Select error" << std::endl;
			exit(1);
		}
		ret = 0;
		for (int i = 0; i < FD_SETSIZE - 2; i++)
		{
			if (FD_ISSET(i, &rset_working))
			{
				//if (i == servers[i].getServerFd())
				if (is_serverfd(i))
				{
					//new_fd = accept((servers[i].getServerFd(), (struct sockaddr *)(servers[i].getAddress()), (socklen_t*)(servers[i].getAddressLen()));
					new_fd = accept(servers[this->getindex(i)].getServerFd(),NULL,NULL);

					if (new_fd > 0)
					{
                        servers[this->getindex(i)].attach(new_fd);
						fcntl(new_fd, F_SETFL, O_NONBLOCK);
						FD_SET(new_fd, &rset_master);
						PRINTN("New connection");
					}
				}
				else
				{
                    std::cout << "Got here "<< std::endl;
                    int server_index = getindexConnex(i); // Find the server with this connexion fd (Saved on a vector since there might be connexions each with a unique fd)
                    int request_index = servers[server_index].getRequestIndex(i); // Once we have our server we get which request is this. if its a -1 then its completely new. Else its already existing but not done yet
					std::cerr << "--> "<<request_index << std::endl;
					char buf[1025] = {0};
					int bytes_read = recv(i, buf, 1024, 0);

                    if ((request_index != -1))
                    {
                        std::cout << "here bitches" << std::endl;
                        servers[server_index].getRequest(request_index).append(buf); 
                    }
                    else
                    {
                        Request request(buf);
                        request.setConnexionFd(i);
                        request.parse();
                        servers[server_index].attach(request);
                    }
                    if (bytes_read == 0 || buf[1023] == 0)
                    {
                        // Once done reading. We set a new reading fd on the server 
                        int server_index = getindexConnex(i); // Find the server with this connexion fd (Saved on a vector since there might be connexions each with a unique fd)
                        std::cout << server_index << std::endl;
                        int request_index = servers[server_index].getRequestIndex(i); // Once we have our server we get which request is this. if its a -1 then its completely new. Else its already existing but not done yet
                        std::cout << request_index << std::endl;

                        //std::cout << "got here 1" << std::endl;
                        std::cout << "meow" << std::endl;

                        servers[server_index].getRequest(request_index).parse();
                       
                        Response response(servers[server_index].getRequest(request_index), servers[server_index]);
                        std::string result = response.build();
                        ret = send(i, result.c_str(), result.length(), 0);

                        // To do
                        
                        //servers[server_index].clean(request_index, i); //

                        close(i);
                        std::cout << "I got here"  << std::endl;
						FD_CLR(i, &rset_master);
						servers[server_index].removeRequest(i);
                    }
				}
		}
		//else if (FD_ISSET(servers[i].getServerFd(), &wset_working))
		else if (FD_ISSET(i, &wset_working))
		{
            std::cout << "gokklkhlht here" << std::endl;
            int server_index = getindexConnex(i); // Find the server with this connexion fd (Saved on a vector since there might be connexions each with a unique fd)
            int request_index = servers[server_index].getRequestIndex(i); // Once we have our server we get which request is this. if its a -1 then its completely new. Else its already existing but not done yet
			Response response(servers[server_index].getRequest(i), servers[server_index]);

            ret = send(i, "Hillo", 5, 0);
			if (ret <= 0)
			{
				exit(EXIT_FAILURE);
			}
			FD_CLR(i, &wset_master);
			close(i);
		}
	}

}

/*


















*/

}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./Webserv [config_file]" << std::endl;
        return 1;
    }
    try
    {
        Webserv webserv((std::string(argv[1])));
        webserv.debug();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}