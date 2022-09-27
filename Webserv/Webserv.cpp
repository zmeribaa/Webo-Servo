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
	for (int i = 0; i < servers.size(); i++)
	{
		FD_SET(servers[i].getServerFd(), &rset_master);
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
				if(is_serverfd(i))
				{
					//new_fd = accept((servers[i].getServerFd(), (struct sockaddr *)(servers[i].getAddress()), (socklen_t*)(servers[i].getAddressLen()));
					new_fd = accept(servers[this->getindex(i)].getServerFd(),NULL,NULL);

					if (new_fd > 0)
					{
						fcntl(new_fd, F_SETFL, O_NONBLOCK);
						FD_SET(new_fd, &rset_master);
						PRINTN("New connection");
					}
				}
				else 
				{
					PRINTN("New request");
					char buf[1024];
					int bytes_read = recv(i, buf, 1024, 0);
					//PRINTN("Bytes read: " << buf);
					if (bytes_read > 0)
					{
						std::string rt(buf);
						Request request(rt);
						//std::cout << rt << std::endl;
						Response response(request, servers[getindex(i)]);
						std::string res = response.build();

						ret = send(i, res.c_str(), res.size(), 0);
						close_conn = TRUE;
						if (close_conn)
						{
							close(i);
							FD_CLR(i, &rset_master);
							close_conn = FALSE;
						}
					}
				}
		}
		//else if (FD_ISSET(servers[i].getServerFd(), &wset_working))
		else if (FD_ISSET(i, &wset_working))
		{
			PRINTN("Write");
			ret = send(i, "Hello", 5, 0);
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