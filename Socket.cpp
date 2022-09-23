/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:36:16 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/23 15:39:42 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <fcntl.h>

// Server side C program to demonstrate HTTP Server programming
Socket::Socket(void)
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	int opt = 1;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 7\n\n3ankoub";
	int ret;
	int fd_ready;
	int new_fd;
	int len;
	char buffer[30000] = {0};
	struct fd_set backup_write;
	struct fd_set write_fds;
	struct fd_set read_fds;
	struct fd_set backup_read;
	int max_fds;
	int close_conn = FALSE;
	FD_ZERO(&backup_read);
	FD_ZERO(&backup_write);
	FD_ZERO(&write_fds);
	FD_ZERO(&read_fds);
	max_fds = server_fd;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
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
	//int	x = 0;

	struct timeval	timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	FD_SET(server_fd, &backup_read);//loop over all servers not just one 
    while(1)
    {
        // printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		read_fds = backup_read;
		write_fds = backup_write;
		// memcpy(&backup_read, &read_fds, sizeof(read_fds));
		// memcpy(&backup_write, &write_fds,sizeof(write_fds));


		ret = select(1024, &read_fds, &write_fds, NULL, &timeout);
		//std::cerr << "RET : " << ret << std::endl;
		//std::cerr << "RET_READ : " << read_fds.fds_bits << std::endl;
		//std::cerr << "RET_WRITE : " << write_fds.fds_bits << std::endl;
		if (ret < 0)
		{
			perror("In select");
			exit(EXIT_FAILURE);
		}
		//fd_ready = ret;
		ret = 0;
		for (int i = 0; i < 1022; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				//fd_ready -= 1;
				//
				if (i == server_fd)
				{
					// std::cout << "Socket is readable" << std::endl;
					new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					std::cout << "NEW_FD : " << new_fd << std::endl;
					if(new_fd > 0)
					{
						
						//break;
						FD_SET(new_fd, &backup_read);
						fcntl(server_fd, F_SETFL, O_NONBLOCK);
					}
					new_fd = -1;
					// if (new_fd > max_fds)
					// 	max_fds = new_fd;
				}
				else
				{
					ret = read( i , buffer, 30000);//recv(i, buffer, sizeof(buffer), 0);
					// std::cout << "RECV_RET : " << i << "|" << ret << std::endl;
					if (ret < 0)
					{
						std::cout << "__BREAAAKINI__" << std::endl;
						close_conn = TRUE;
						//break;
					}
					if (ret == 0)
					{
						std::cout << "RECV_RET : " << i << "|" << ret << std::endl;
						FD_CLR(i, &backup_read);
						FD_SET(i, &backup_write);
					}
					
					//len = ret;
					if (close_conn)
					{
						close(i);
						FD_CLR(i, &backup_read);
						close_conn = FALSE;
						// if (i == max_fds)
						// {
						// 	while(FD_ISSET(max_fds, &read_fds) == FALSE)
						// 		max_fds -= 1;
						// }
					}
				}
			}
			else if (FD_ISSET(i, &write_fds))
			{
				//fd_ready -= 1;
				ret =  send(i , hello.c_str(), hello.length(), 0);//send(i, hello.c_str(), hello.length(), 0);
				if (ret <= 0)
				{
					//perror("In send");
					//exit(EXIT_FAILURE);
				}
				FD_CLR(i, &backup_write);
				close(i);
				std::cout << "SEND_RET : " << i << "|" << ret << std::endl;
				// if (i == max_fds)
				// {
				// 	while(FD_ISSET(max_fds, &write_fds) == FALSE)
				// 		max_fds -= 1;
				// }
			}
		}
        // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        // {
        //     perror("In accept");
        //     exit(EXIT_FAILURE);
		// }
        // valread = read( new_socket , buffer, 30000);
        // // printf("%s\n",buffer );
		// // std::cerr << "Res To :[" << new_socket << "]" << std::endl;
		// // if (true) {
		// // 	while (true) {}
		// // }
        // send(new_socket , hello.c_str(), hello.length(),0);
		
        // // printf("------------------Hello message sent-------------------");
        // close(new_socket);
    }
}

Socket::~Socket(){}

int main(int argc, char const *argv[])
{
	signal(SIGPIPE, SIG_IGN);

	Socket *socket = new Socket();
	return 0;
}