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

#include "../Webserv/Webserv.hpp"
#include "../Request/Request.hpp"

class Response {
	public:
		Response(void);
		Response(Request request, Server server);
		Response(std::string version, std::string code, std::string phrase);
		~Response(void);
		void appendHeader(std::string header);
		void setContentTypes();
		std::string extToMime(std::string key);
		void addBody(std::string body);
		std::string build();
	private:
        std::unordered_map<std::string, std::string> keys;
		std::unordered_map<std::string, std::string> content_type;
		std::string res;
};