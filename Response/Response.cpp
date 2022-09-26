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
    content_type[".aac"] = "audio/aac";
    content_type[".abw"] = "application/x-abiword";
    content_type[".arc"] = "application/x-freearc";
    content_type[".avif"] = "image/avif";
    content_type[".avi"] = "video/x-msvideo";
    content_type[".azw"] = "application/vnd.amazon.ebook";
    content_type[".bin"] = "application/octet-stream";
    content_type[".bmp"] = "image/bmp";
    content_type[".bz"] = "application/x-bzip";
    content_type[".bz2"] = "application/x-bzip2";
    content_type[".cda"] = "application/x-cdf";
    content_type[".csh"] = "application/x-csh";
    content_type[".css"] = "text/css";
    content_type[".csv"] = "text/csv";
    content_type[".doc"] = "application/msword";
    content_type[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    content_type[".eot"] = "application/vnd.ms-fontobject";
    content_type[".epub"] = "application/epub+zip";
    content_type[".gz"] = "application/gzip";
    content_type[".gif"] = "image/gif";
    content_type[".htm"] = "text/html";
    content_type[".html"] = "text/html";
    content_type[".ico"] = "image/vnd.microsoft.icon";
    content_type[".ics"] = "text/calendar";
    content_type[".jar"] = "application/java-archive";
    content_type[".jpeg"] = "image/jpeg";
    content_type[".jpg"] = "image/jpeg";
    content_type[".js"] = "text/javascript";
    content_type[".json"] = "application/json";
    content_type[".jsonld"] = "application/ld+json";
    content_type[".midi"] = "audio/midi";
    content_type[".mid"] = "audio/midi";
    content_type[".mjs"] = "text/javascript";
    content_type[".mp3"] = "audio/mpeg";
    content_type[".mp4"] = "video/mp4";
    content_type[".mpeg"] = "video/mpeg";
    content_type[".mpkg"] = "application/vnd.apple.installer+xml";
    content_type[".odp"] = "application/vnd.oasis.opendocument.presentation";
    content_type[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    content_type[".odt"] = "application/vnd.oasis.opendocument.text";
    content_type[".oga"] = "audio/ogg";
    content_type[".ogv"] = "video/ogg";
    content_type[".ogx"] = "application/ogg";
    content_type[".opus"] = "audio/opus";
    content_type[".otf"] = "font/otf";
    content_type[".png"] = "image/png";
    content_type[".pdf"] = "application/pdf";
    content_type[".php"] = "application/x-httpd-php";
    content_type[".ppt"] = "application/vnd.ms-powerpoint";
    content_type[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    content_type[".rar"] = "application/vnd.rar";
    content_type[".rtf"] = "application/rtf";
    content_type[".sh"] = "application/x-sh";
    content_type[".svg"] = "image/svg+xml";
    content_type[".tar"] = "application/x-tar";
    content_type[".tiff"] = "image/tiff";
    content_type[".tiff"] = "image/tiff";
    content_type[".ts"] = "video/mp2t";
    content_type[".ttf"] = "font/ttf";
    content_type[".txt"] = "text/plain";
    content_type[".vsd"] = "application/vnd.visio";
    content_type[".wav"] = "audio/wav";
    content_type[".weba"] = "audio/webm";
    content_type[".webm"] = "video/webm";
    content_type[".webp"] = "image/webp";
    content_type[".woff"] = "font/woff";
    content_type[".woff2"] = "font/woff2";
    content_type[".xhtml"] = "application/xhtml+xml";
    content_type[".xls"] = "application/vnd.ms-excel";
    content_type[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    content_type[".xml"] = "application/xml";
    content_type[".xul"] = "application/vnd.mozilla.xul+xml";
    content_type[".zip"] = "application/zip";
    content_type[".3gp"] = "video/3gpp";
    content_type[".3g2"] = "video/3gpp2";
    content_type[".7z"] = "application/x-7z-compressed";
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


std::string Response::fullPathBuilder(Request request, Location location)
{
    if (location.getKey("path").back() == '/')
       return (location.getKey("path") + request.getKey("path").substr(location.getPath().length())); // This shit is ugly return this from findLocation later
    else
        return (location.getKey("path") + "/" + request.getKey("path").substr(location.getPath().length())); // This shit is ugly return this from findLocation later
}

Response::Response(Request request, Server server)
{
    setContentTypes();
    Location *location = server.findLocation(request.getKey("path"));

    if (location == NULL)
        buildError("404");
    else
    {
        std::string full_path = fullPathBuilder(request, *location);
        std::cout << "Full constructed path is: " << full_path << std::endl;
        struct stat s;
        if (stat(full_path.c_str(), &s) != 0)
            buildError("404");
        else
        {
            if (s.st_mode & S_IFDIR)
            {
                int auto_index = 0;
                if (auto_index == 1)
                {
                    // Show autoindex
                }
                else
                {
                    if (full_path.back() == '/')
                        full_path += "index.html";
                    else
                        full_path += "/index.html";
                    if (stat(full_path.c_str(), &s) == 0)
                        serveStaticContent(full_path);
                    else
                        buildError("404");
                }
                }
                else
                    serveStaticContent(full_path);
            }
    }
}

Response::~Response(void)
{
}

void Response::buildError(std::string error_type)
{
    // To improve later
    if (error_type == "404")
    {
        keys["version"] = "HTTP/1.1";
        keys["code"] = "404";
        keys["phrase"] = "Not found";

        keys["body"] = "<h1> Shit is 404 </h1>";

        appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
        appendHeader("Content-Type: text/html");
    }
}

void Response::serveStaticContent(std::string full_path)
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
