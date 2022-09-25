/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:12:34 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/25 18:57:25 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"


cgi::cgi(){}

cgi::cgi(std::string path, std::string type, Request req): , _path(path), _type(type), _req(req){}

cgi::~cgi(){}

void cgi::env()
{
	std::map<std::string, std::string>::iterator it = req.find("reqtype");
	if (it != req.end())
	{
		if (it->second == "GET")
		{
			
		}
	}
}

