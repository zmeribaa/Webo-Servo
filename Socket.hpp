/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:37:33 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/20 01:44:29 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 80

class Socket {
public:
	Socket(void);
	~Socket(void);
	class SocketException : public std::exception{
		private:
			std::string _err;
		public:
			SocketException(std::string const &) throw() : _err(err){}
			virtual ~SocketException() throw(){}
			virtual const char *what() const throw(){ return _err;}
	};
};