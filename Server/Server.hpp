#ifndef SERVER_HPP
# define SERVER_HPP

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
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1




#pragma once


#include "../Location/Location.hpp"
#include "../Request/Request.hpp"

// Create Server class with all nginx configuration parameters

class Server {
    public:
        Server();
        //~Server();
        void attach(const Location location);
        bool attach(std::string pair);
		void attach(int fd);
		void detach(int fd);

        void debug();
        void lessgo();
        void run();
        void _socket();
        void setName(std::string name);
		int getServerFd();
		void setServerFd(int fd);
		struct sockaddr_in getAddress();
		void setAddress(struct sockaddr_in address);
		socklen_t	getAddressLen();
		void setAddressLen(socklen_t address_len); 
        Location *findLocation(std::string path);
		void setConnexionFd(int fd);
void clean(int request_index, int fd);

		int getConnexFd(int fd);
		void attach(Request request);
		Request getRequest(int i);
		int	getRequestIndex(int fd);
		void	removeRequest(int fd);

    private:
        std::unordered_map<std::string, std::string> keys;
        std::string _name;
        std::vector<Location> locations;
        std::vector<Request> requests;
        struct sockaddr_in address;
        int server_fd;
        struct fd_set backup_write;
	    struct fd_set write_fds;
	    struct fd_set read_fds;
	    struct fd_set backup_read;
		socklen_t address_len;
		std::vector<int> connexion_fds;
};

#endif

// while (1)
//     {
//         for (int i = 0; i < servers.size(); i++)
//             servers[i].run();
	

// read_fds = backup_read;
// 		write_fds = backup_write;
// 		ret = select(1024, &read_fds, &write_fds, NULL, &timeout);
// 		if (ret < 0)
// 		{
// 			perror("In select");
// 			exit(EXIT_FAILURE);
// 		}
// 		ret = 0;
// 		for (int i = 0; i < 1022; i++)
// 		{
// 			if (FD_ISSET(i, &read_fds))
// 			{			
// 				if (i == server_fd)
// 				{
// 					new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
// 					std::cout << "NEW_FD : " << new_fd << std::endl;
// 					if(new_fd > 0)
// 					{
// 						fcntl(new_fd, F_SETFL, O_NONBLOCK);
// 						FD_SET(new_fd, &backup_read);
// 					}
// 				}
// 				else
// 				{
// 					char buffer[30000] = {0};
//         			int valread = recv(new_fd , buffer, 30000,0);
					
// 					// Too ugly, to refactor later
// 					if (valread > 0)
//        				{
// 						std::string rt(buffer);
//         			Request request(rt);
// 					Response response(request, *this);

// 					std::string res = response.build();

// 					ret =  send(i , res.c_str(), res.length(), 0);
// 					close_conn = TRUE;
// 					if (close_conn)
// 					{
// 						//request.debug();
// 						close(i);
// 						FD_CLR(i, &backup_read);
// 						close_conn = FALSE;
// 					}
// 					}
// 				}
// 			}
// 			else if (FD_ISSET(i, &write_fds))
// 			{
// 				ret =  send(i , hello.c_str(), hello.length(), 0);
// 				if (ret <= 0)
// 				{
// 					exit(EXIT_FAILURE);
// 				}
// 				FD_CLR(i, &backup_write);
// 				close(i);
// 				std::cout << "SEND_RET : " << i << "|" << ret << std::endl;
// 			}
// 		}

	/*		
		wset-master;
		rset-master
		bzero(writting || reading)
		

		foreach-server
			{
						fdset( 
					adding all server socket to read
				)
			}



		while (1)
		{
			working-reading = master reading
			woroking-writing = master writing



			select(wr, ww)
			for (fd to 1024)
			{
				if is set(read)
				{
					if (its server)
					{
						con = accept (fd-server)
						ds.{fdserver , fdconnect}
						rset-master.add(con)
					}
					if (its con)
					{
						reding 
					}
					if (done reading)
					{
						remove from read set
						add it to write
					}
				}
				if (is set to write)
				{
					if (is still wrting)
					{
						do ur job
						mapconnection[con] = serverid
						servers[serverfd] .is allowd;
					}
					else done
					{
						shutdown (socket)
						close (socket)
						wrting-master remove (Socket)
					}
				}
			}
		}

	
    

    }*/