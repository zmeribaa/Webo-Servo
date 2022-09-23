/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:36:16 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/23 02:05:11 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

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
	int i = 0;
	char buffer[30000] = {0};
	struct fd_set backup_write;
	struct fd_set write_fds;
	struct fd_set read_fds;
	struct fd_set backup_read;
	int max_fds;
	int close_conn = FALSE;
	FD_ZERO(&backup_read);
	FD_ZERO(&backup_write);
	FD_SET(server_fd, &backup_read);
	max_fds = server_fd;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		memcpy(&read_fds, &backup_read, sizeof(read_fds));
		memcpy(&write_fds, &backup_write, sizeof(write_fds));

		FD_SET(server_fd, &read_fds);

		ret = select(1024, &read_fds, &write_fds, NULL, NULL);
		std::cout << ret << std::endl;
		if (ret < 0)
		{
			perror("In select");
			exit(EXIT_FAILURE);
		}
		fd_ready = ret;
		for (i = 0; i < 1024; ++i)
		{
			if (FD_ISSET(server_fd, &read_fds))
			{
				fd_ready -= 1;
				if (i == server_fd)
				{
					std::cout << "Socket is readable" << std::endl;
					new_fd = accept(server_fd, NULL, NULL);
					if(new_fd < 0)
						break;
					FD_SET(new_fd, &read_fds);
					if (new_fd > max_fds)
						max_fds = new_fd;
				}
				else
				{
					ret = recv(i, buffer, sizeof(buffer), 0);
					if (ret <= 0)
					{
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
			else if (FD_ISSET(i, &write_fds))
			{
				fd_ready -= 1;
				ret = send(i, hello.c_str(), hello.length(), 0);
				if (ret < 0)
				{
					perror("In send");
					exit(EXIT_FAILURE);
				}
				close(i);
				FD_CLR(i, &write_fds);
				if (i == max_fds)
				{
					while(FD_ISSET(max_fds, &write_fds) == FALSE)
						max_fds -= 1;
				}
			}
		}
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
		}
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        send(new_socket , hello.c_str(), hello.length(),0);
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
}

Socket::~Socket(){}

int main(int argc, char const *argv[])
{
	Socket *socket = new Socket();
	return 0;
}