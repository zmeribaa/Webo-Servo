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


// Path: /test/meow.php?wewe=meow

void Response::fullPathBuilder(std::string url, Location location)
{
    std::string::size_type pos;
    std::string full_path;

    pos = url.find("?");
	if (pos != std::string::npos)
	{
		keys["full_file_path"] = url.substr(0, pos);
		keys["query"] = url.substr(pos + 1, url.length() - 1);
	}
	else
		keys["full_file_path"] = url;

    if (location.getKey("path").back() == '/')
      keys["full_file_path"] = location.getKey("path") + keys["full_file_path"].substr(location.getPath().length()); // This shit is ugly return this from findLocation later
    else
        keys["full_file_path"] = location.getKey("path") + "/" + keys["full_file_path"].substr(location.getPath().length()); // This shit is ugly return this from findLocation later

    struct stat s;
    if (stat(keys["full_file_path"].c_str(), &s) != 0)
        keys["code"] = "404";
    else
    {
        if (s.st_mode & S_IFDIR)
        {
            std::string index = location.getKey("default").empty() ? location.getKey("default") : "index.html";
            std::string path_backup = keys["full_file_path"];
            if (keys["full_file_path"].back() == '/')
                keys["full_file_path"] += index;
            else
                keys["full_file_path"] += "/" + index;
            if (stat(keys["full_file_path"].c_str(), &s) != 0)
            {
                if (location.getKey("directory_listing") == "true")
                {
                    keys["directory_listing"] = "true";
                    keys["full_file_path"] = path_backup;
                }
                else
                    keys["code"] = "403";
            }
                
        }
    }
}



// Goal for path /trololo/filename.php
void Response::setMetaData(Request request, Server server)
{
    Location *location = server.findLocation(request.getKey("path"));
    if (location == NULL)
        keys["code"] = "404";
    else
    {
        fullPathBuilder(request.getKey("path"), *location);
        if (keys["code"].empty())
        {

            std::string::size_type pos;
            pos = keys["full_file_path"].find_last_of(".");
            if (pos != std::string::npos && pos != keys["full_file_path"].length())
            {
                keys["extension"] = keys["full_file_path"].substr(pos);
                std::string cgi = location->getKey("cgi");
                std::cout << "extension " << keys["extension"] << std::endl;

                if ((pos = cgi.find(",")) != std::string::npos)
                {
                    std::string cgi_extension = cgi.substr(0, pos);
                    std::cout << "CGI extension is " << cgi_extension << std::endl;
                    if (keys["extension"] == cgi_extension)
                        keys["cgi_path"] = cgi.substr(pos + 1);
                }
            }
        }
    }
}

void Response::serveDirectoryListing(Request request)
{
        keys["version"] = "HTTP/1.1";
        keys["code"] = "200";
        keys["phrase"] = "Found";

        keys["body"] = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\"><html> <head> <title>Index of " + request.getKey("path") + "</title> </head> <body><h1>Index of " + request.getKey("path") + "  </h1> <table> <tr><th valign=\"top\">&nbsp;</th><th>Name</th><th>Last modified</th><th>Size</th><th>Description</th></tr><tr><th colspan=\"5\"><hr></th></tr><tr><td valign=\"top\">&nbsp;</td>";
        
        // To do: Description, file sizes, and last modified

        DIR *dir;
        struct dirent *dp;

        if ((dir = opendir (keys["full_file_path"].c_str())) == NULL)
        {
            perror ("Cannot open .");
        }

        while ((dp = readdir (dir)) != NULL)
        {
            std::string name(dp->d_name);
            keys["body"] += "<tr> \
            <td valign=\"top\">&nbsp;</td><td><a href=\"" + name + "\">"  + name + "</a> </td> \
            <td align=\"right\">" + "2022" + "</td> \
            <td align=\"right\">" + std::to_string(dp->d_reclen) + "</td> \
            <td>Description</td> \
            </tr>";
        }
        
        keys["body"] += "<td>&nbsp;</td></tr><tr><th colspan=\"5\"><hr></th></tr></table></body></html>";

        appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
        appendHeader("Content-Type: text/html");
}

/*void    Response::serveCgi(Request request)
{
    //std::string reqtype = _req.getKey("reqtype");

	// There will always be a reqtype; so no need to check here. But a check might be done getKey level either throw an exception ot check if empty()
	if (request.getKey("reqtype") == "GET")
	{
		std::string query = keys["query"];	
		if (query.size() != 0)
		{
			std::string c_size = std::to_string(keys["query"].length());
			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}		
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (request.getKey("reqtype") == "POST")
	{
		std::string content_type = request.getKey("Content-Type");
		if (!(content_type.empty()))
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::string content_length = request.getKey("Content-Length");
		if (!(content_length.empty()))
			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	}
    
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", keys["query"].c_str(), 1);
	setenv("REQUEST_METHOD", reqtype.c_str(), 1);
	setenv("SCRIPT_FILENAME", keys["full_file_path"], 1); // Parse file on request level
	//setenv("SERVER_SOFTWARE", "LOLOLOLOLOL", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}*/


Response::Response(Request request, Server server)
{
    setContentTypes();


    if (request.getKey("reqtype") == "GET")
    {
        setMetaData(request, server);
        if (keys["directory_listing"] == "true")
        {
            serveDirectoryListing(request);
        }
        else if (!(keys["code"].empty()))
        {
            buildError(keys["code"]);
        }
        else if (!(keys["cgi_path"].empty()))
        {
            // Handle CGI shit here
        }
        else
        {
            serveStaticContent(keys["full_file_path"]);
        }
    } // GET request
    /*else if (request.getKey("reqtype") == "POST")
        // POST
    else if (request.getKey("reqtype") == "POST")
        // DELETE*/
}



Response::~Response(void)
{
}

void Response::buildError(std::string error_type)
{
    // To improve later

        keys["version"] = "HTTP/1.1";
        keys["code"] = error_type;
        keys["phrase"] = "Not found";

        keys["body"] = "<h1> Shit is 404 </h1>";

        appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
        appendHeader("Content-Type: text/html");
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
