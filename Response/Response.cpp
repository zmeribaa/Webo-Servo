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
#include <sys/stat.h>

Response::Response(void)
{
}

// Later move this to  server or webserv class
void Response::setContentTypes()
{
	keys[".aac"] = "audio/aac";
	keys[".abw"] = "application/x-abiword";
	keys[".arc"] = "application/x-freearc";
	keys[".avif"] = "image/avif";
	keys[".avi"] = "video/x-msvideo";
	keys[".azw"] = "application/vnd.amazon.ebook";
	keys[".bin"] = "application/octet-stream";
	keys[".bmp"] = "image/bmp";
	keys[".bz"] = "application/x-bzip";
	keys[".bz2"] = "application/x-bzip2";
	keys[".cda"] = "application/x-cdf";
	keys[".csh"] = "application/x-csh";
	keys[".css"] = "text/css";
	keys[".csv"] = "text/csv";
	keys[".doc"] = "application/msword";
	keys[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	keys[".eot"] = "application/vnd.ms-fontobject";
	keys[".epub"] = "application/epub+zip";
	keys[".gz"] = "application/gzip";
	keys[".gif"] = "image/gif";
	keys[".htm"] = "text/html";
	keys[".html"] = "text/html";
	keys[".ico"] = "image/vnd.microsoft.icon";
	keys[".ics"] = "text/calendar";
	keys[".jar"] = "application/java-archive";
	keys[".jpg"] = "image/jpeg";
	keys[".jpg"] = "image/jpeg";
	keys[".js"] = "text/javascript (Specifications: HTML and RFC 9239)";
	keys[".json"] = "application/json";
	keys[".jsonld"] = "application/ld+json";
	keys[".midi"] = "audio/midi audio/x-midi";
	keys[".mid"] = "audio/midi audio/x-midi";
	keys[".mjs"] = "text/javascript";
	keys[".mp3"] = "audio/mpeg";
	keys[".mp4"] = "video/mp4";
	keys[".mpeg"] = "video/mpeg";
	keys[".mpkg"] = "application/vnd.apple.installer+xml";
	keys[".odp"] = "application/vnd.oasis.opendocument.presentation";
	keys[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	keys[".odt"] = "application/vnd.oasis.opendocument.text";
	keys[".oga"] = "audio/ogg";
	keys[".ogv"] = "video/ogg";
	keys[".ogx"] = "application/ogg";
	keys[".opus"] = "audio/opus";
	keys[".otf"] = "font/otf";
	keys[".png"] = "image/png";
	keys[".pdf"] = "application/pdf";
	keys[".php"] = "application/x-httpd-php";
	keys[".ppt"] = "application/vnd.ms-powerpoint";
	keys[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	keys[".rar"] = "application/vnd.rar";
	keys[".rtf"] = "application/rtf";
	keys[".sh"] = "application/x-sh";
	keys[".svg"] = "image/svg+xml";
	keys[".tar"] = "application/x-tar";
	keys[".tiff"] = "image/tiff";
	keys[".tiff"] = "image/tiff";
	keys[".ts"] = "video/mp2t";
	keys[".ttf"] = "font/ttf";
	keys[".txt"] = "text/plain";
	keys[".vsd"] = "application/vnd.visio";
	keys[".wav"] = "audio/wav";
	keys[".weba"] = "audio/webm";
	keys[".webm"] = "video/webm";
	keys[".webp"] = "image/webp";
	keys[".woff"] = "font/woff";
	keys[".woff2"] = "font/woff2";
	keys[".xhtml"] = "application/xhtml+xml";
	keys[".xls"] = "application/vnd.ms-excel";
	keys[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	keys[".xml"] = "application/xml";
	keys[".xul"] = "application/vnd.mozilla.xul+xml";
	keys[".zip"] = "application/zip";
	keys[".3gp"] = "video/3gpp";
	keys[".3g2"] = "video/3gpp2";
	keys[".7z"] = "application/x-7z-compressed";
}

std::string Response::extToMime(std::string key)
{
	if (content_type.find(key) != content_type.end())
		return content_type[key];
	else
		return ("application/octet-stream");
}

Response::Response(std::string version, std::string code, std::string phrase)
{
	keys["version"] = version;
	keys["code"] = code;
	keys["phrase"] = phrase;
}


// A draft response construtor. needs refactoring


Response::Response(Request request, Server server)
{
	setContentTypes();
	// Shitton of logic to have here
	//	keys["version"] = "HTTP/1.1";
	//	keys["code"] = "200";
	//	keys["phrase"] = "OK";

	//	keys["body"] = "<h1>Received a " + request.getKey("reqtype") + " request, to path " + request.getKey("path") + " on http version " +  request.getKey("vers") + "</h1>";
	//	keys["body"] += request.htmlHeaders();

	// appendHeader("Content-Length: " + std::to_string(	keys["body"].length()));
	// appendHeader("Content-Type: text/html");

	// Goal serve static html

	Location *location = server.findLocation(request.getKey("path"));

	if (location == NULL)
	{
		// We got a 404

		keys["version"] = "HTTP/1.1";
		keys["code"] = "404";
		keys["phrase"] = "Not found";

		keys["body"] = "<h1> Shit is 404 </h1>";

		appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
		appendHeader("Content-Type: text/html");
	}
	else
	{
		std::string full_path = location->getKey("path") + request.getKey("path").substr(location->getPath().length()); // This shit is ugly return this from findLocation later
		if (full_path.back() != '/')
			full_path += "/";
		std::cout << "Full constructed path is: " << full_path << std::endl;
		struct stat s;
		if (stat(full_path.c_str(), &s) != 0)
		{
			keys["version"] = "HTTP/1.1";
			keys["code"] = "404";
			keys["phrase"] = "Not found";

			keys["body"] = "<h1> Shit is 404 </h1>";

			appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
			appendHeader("Content-Type: text/html");
		}
		else
		{
			if (s.st_mode & S_IFDIR)
			{
				if (full_path.back() == '/')
					full_path += "index.html";
				else
					full_path += "/index.htm";
				if (stat(full_path.c_str(), &s) == 0)
				{
					keys["version"] = "HTTP/1.1";
					keys["code"] = "200";
					keys["phrase"] = "OK";

					std::ifstream t(full_path);

					std::stringstream _buffer;
					_buffer << t.rdbuf();
					appendHeader("Content-Length: " + std::to_string(_buffer.str().length()));
					appendHeader("Content-Type: text/html");
					addBody(_buffer.str());
				}
				else // Later on check if there's auto index and either return the files or throw 403
				{
					keys["version"] = "HTTP/1.1";
					keys["code"] = "404";
					keys["phrase"] = "Not found";

					keys["body"] = "<h1>Shit is 404</h1>";

					appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
					appendHeader("Content-Type: text/html");
				}
			}
			else
			{
				keys["version"] = "HTTP/1.1";
				keys["code"] = "200";
				keys["phrase"] = "OK";

				std::ifstream t(full_path);

				std::stringstream _buffer;
				_buffer << t.rdbuf();
				appendHeader("Content-Length: " + std::to_string(_buffer.str().length()));
				int pos;
				if ((pos = full_path.find_last_of('.')) != std::string::npos)
					appendHeader("Content-Type: " + extToMime(full_path.substr(pos)));
				else
					appendHeader("Content-Type: text/plain");
				addBody(_buffer.str());
			}
		}
	}
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

	res = status_line + "\r\n" + keys["headers"];
	if (keys.find("body") != keys.end())
		res += "\r\n" + keys["body"];
	return (res);
}
