/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SmolServer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:36:16 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/12 18:53:47 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SmolServer.h"

int main(int ac, char **av)
{
	const int PORT = 8080;
	int server_fd;
	char *hello;
	char buffer[1024] = {0};
	long valread;
	int new_socket;
	struct sockaddr_in server_addr;
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror( "socket failed" );
		exit(EXIT_FAILURE);
	}
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror( "bind failed" );
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror( "In Listen" );
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&server_addr,(socklen_t *)&server_addr)) < 0)
	{
		perror( "In Accept" );
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	if (valread < 0)
	{
		printf("No Bytes are there to read");
	}
	hello = "Hello from the server";
	write(new_socket, hello, strlen(hello));
	close(new_socket);
	return 0;
}
