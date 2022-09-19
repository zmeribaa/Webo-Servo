/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 14:26:42 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/17 16:45:02 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

class Response {
	private:
		std::string _Headers;
		std::string _Body;
		// std::string _Status;
		// std::string _Version;
		// std::string _ContentType;
		// std::string _ContentLength;
		std::string _Date;
		std::string _Server;
		
	public:
		Response(void);
		~Response(void);
		void setHeaders(std::string);
		void setBody(std::string);
		void setStatus(std::string);
		void setVersion(std::string);
		void setContentType(std::string);
		void setContentLength(std::string);
		void setDate(std::string);
		void setServer(std::string);
		std::string getHeaders(void);
		std::string getBody(void);
		std::string getStatus(void);
		std::string getVersion(void);
		std::string getContentType(void);
		std::string getContentLength(void);
		std::string getDate(void);
		std::string getServer(void);
};