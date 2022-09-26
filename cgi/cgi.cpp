/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:12:34 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/26 06:08:06 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi(){}

cgi::cgi(std::string path, std::string type, Request req, std::string file):  _path(path), _type(type), _req(req), _file(file){}

cgi::~cgi(){}

void cgi::env()
{
	std::map<std::string, std::string>::iterator it = _req.find("_reqtype");
	if (it != _req.end())
	{
		if (it->second == "GET")
		{
			int s = _req.length();
			std::string c_size = toString(s);
			if (_req._query.size() != 0)
				setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (it->second == "POST")
	{
		std::map<std::string, std::string>::iterator ite = _req.find("content-type");
		if (ite != _req.end())
			setenv("CONTENT_TYPE", (ite->second)->c.str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::map<std::string, std::string>::iterator iter = _req.find("content-length");
		if (iter != _req.end())
			setenv("CONTENT_LENGTH", (iter->second).c_str(), 1);
	}
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", _req._query.c_str(), 1);
	setenv("_reqUEST_METHOD", (it->first).c_str(), 1);
	setenv("SCRIPT_FILENAME", _file.c_str(), 1);
	//setenv("SERVER_SOFTWARE", /*same as the server description*/, 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

