/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 14:30:41 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/17 15:09:51 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
}

Response::Response(std::string version, std::string code, std::string phrase)
{
	keys["version"] = version;
	keys["code"] = code;
	keys["phrase"] = phrase;
}

Response::~Response(void)
{

}

void Response::appendHeader(std::string header)
{
	keys["headers"] += header + "\r\n";
}

void Response::addBody(std::string body)
{
	keys["body"] = body;
}


std::string Response::build()
{
	std::string status_line = keys["version"] + " " + keys["code"] + " " + keys["phrase"];

	res = status_line +  "\r\n" + keys["headers"];
	if (keys.find("body") != keys.end())
		res += "\r\n" + keys["body"];
	return (res);
}
