#include <stdio.h>
#include "Server.hpp"
#include "Request.hpp"
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define FALSE 0
#define TRUE 1

Server::Server()
{
    //std::cout << "We've got a server" << std::endl;
}


// void Server::debug()
// {
//     std::cout << std::endl << "Server " << _name << " config:" << std::endl; 

//     std::unordered_map<std::string, std::string>::iterator it = keys.begin();

//     // Iterate over the map using iterator
//     while(it != keys.end())
//     {
//         std::cout << it->first << " :: " << it->second<<std::endl;
//         it++;
//     }

//     std::vector<Location>::iterator ptr;
//     for (ptr = locations.begin(); ptr != locations.end(); ++ptr)
//         ptr->debug();

//     /*std::cout << "Locations config:" << std::endl; 
//     for (auto it = begin(locations); it != end(locations); ++it)
//         it->debug();*/
// }

// bool Server::attach(std::string pair)
// {
//     std::vector<std::string> pairs = split(pair, ':');
//     if (!pairs.empty())
//         keys[pairs[0]] = pairs[1];
//     else
//        std::cout << "This shit so empty" << std::endl;
//     return 1;
// }

void Server::lessgo()
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	struct fd_set  backup_write;
	struct fd_set	write_fds;
	struct fd_set	read_fds;
	struct fd_set 	backup_read;
	int ret, fd_ready, new_fd;
	int len;
	int end_server = FALSE;
	int i = 0;
	char buffer[30000] = {0};
	int opt = 1;
	int max_fds;
	int close_conn;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 54623\n\n Hello";
	
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // max_fds = server_fd;
	// fcntl(server_fd, F_SETFL, O_NONBLOCK);

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
    if (listen(server_fd, 1024) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

	FD_ZERO(&backup_read);
	FD_ZERO(&write_fds);
	FD_ZERO(&read_fds);
	FD_ZERO(&server_fd);
	FD_ZERO(&backup_write);
	max_fds = server_fd;
// map<int, Request> request_mapper; (client_fd, req);
	FD_SET(server_fd, &read_fds);
    while(end_server == FALSE)
    {
//		read_fds = write_fds = backup_fds;
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		memcpy(&read_fds, &backup_read, sizeof(read_fds));
		memcpy(&write_fds, &backup_write, sizeof(write_fds));
		ret = select(max_fds + 1, &read_fds, &write_fds, NULL, NULL);
		if (ret < 0)
		{
			perror("Select error");
			exit(EXIT_FAILURE);
		}
		fd_ready = ret;
		for(i = 0; i < max_fds && fd_ready > 0; ++i)
		{
			if (FD_ISSET(i, &read_fds))
			{
				fd_ready -= 1;
				if (i == server_fd)
				{
					std::cout << "Socket is readable" << std::endl;
					new_fd = 0;
					while(new_fd != -1)
					{
						new_fd = accept(server_fd, NULL, NULL);
						if (new_fd < 0)
						{
							if(errno != EWOULDBLOCK)
							{
								perror("accept() failed");
								end_server = TRUE;
							}
							break;
						}
						FD_SET(new_fd, &read_fds);
						if (new_fd > max_fds)
							max_fds = new_fd;
					}
				}
				else
				{
					while (1)
					{
						ret = recv(i, buffer, sizeof(buffer), 0);
						if (ret < 0)
						{
							// if (errno != EWOULDBLOCK)
							// {
							// 	perror("recv() failed");
							// 	close_conn = TRUE;
							// }
							// break;

						}
						if (ret == 0)
						{
							std::cout << "Connection close" << std::endl;
							close_conn = TRUE;
							break;
						}
						len = ret;
					}
					
					if (close_conn)
					{
						close(i);
						FD_CLR(i, &read_fds);
						if (i == max_fds)
						{
							while(FD_ISSET(max_fds, &read_fds) == FALSE)
								max_fds -= 1;
						}
					}
				}
			}
			else if (FD_ISSET(i, &write_fds)) {

					// if set to write 
					// write to socket
					// ret = send(i, buffer, len, 0);
					// if (ret < 0)
					// // {
					// // 	perror("send() failed");
					// // 	close_conn = TRUE;
					// // 	break;
					// // }
			}
		}
// //			select call; select(max_fds, read_fds, write_fds, NULL, NULL);

// //          for i in max_fds:				
// 	//				i inclus read_fds: (FD_ISSET(i, read_fds))
// 	//					if i == server_fd:
// 							// accept new connection
// 							// add to set FD_SET(new_fd, read_fds);
// 						//else
// 						//      recv(i, buff)
// 						//      if (map.find())
// 	//				i inclus write_fds:
// 	//					i is a client socket => write response
// //				
	}
		for (i = 0; i <= max_fds; ++i)
		{
			if (FD_ISSET(i, &read_fds))
				close(i);
		}
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
		// set socket as non blocking
		// fcntl(new_socket, F_SETFL, O_NONBLOCK);
        
        valread = recv( new_socket , buffer, 30000,0);

        std::string rt(buffer);
        Request request(rt);

        send(new_socket , hello.c_str() , hello.length(),0);

        close(new_socket);
    }

void Server::setName(std::string name)
{
    _name = name; 
}


void Server::attach(const Location location)
{
    locations.push_back(location);
}

int main()
{
	Server server;
	server.lessgo();
}