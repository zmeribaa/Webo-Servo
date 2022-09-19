#include "Request.hpp"

Request::Request()
{
    //std::cout << "We've got a Request" << std::endl;
}

Request::Request(std::string rt)
{
    std::stringstream to_stream(rt);

    std::string line;
    std::getline(to_stream, line);
    line.resize(line.size() - 1);

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    if ((pos = rt.find("\r\n\r\n")) != std::string::npos) // has body check
        keys["body"] = rt.substr(pos + 4);

    pos = line.find(' ', prev);
    keys["reqtype"] = line.substr(0, pos - prev);
    prev = pos + 1;

    pos = line.find(' ', prev);
    keys["path"] = line.substr(prev, pos - prev);
    prev = pos + 1;
	
    pos = line.find(' ', prev);
    keys["vers"] = line.substr(prev);

    std::cout << "Received a <<" << keys["reqtype"] << ">> 	request, to path <<" << keys["path"] << ">> on http version <<" << keys["vers"] << ">>" << std::endl;
    
    //std::cout << "Request body <<" << keys["body"] << ">>" << std::endl;

    while (std::getline(to_stream, line))
    {
        line.resize(line.size() - 1);
        pos = line.find(':');
        if (pos != -1)
        {
            //std::cout << line.substr(0, pos) << " :: " << line.substr(pos + 2) << std::endl; 
            keys[line.substr(0, pos)] = line.substr(pos + 2);
        }
    }

    if (keys["Transfer-Encoding"].find("chunked") != std::string::npos)
    {
        std::cout << "Shit this body is chunked" << std::endl;
        std::string decoded_body;
        int length = 0;
        unsigned long chunk_size = strtol(keys["body"].c_str(), NULL, 16);
        while (chunk_size > 0)
        {
            length = keys["body"].find("\r\n", length) + 2;
            decoded_body += keys["body"].substr(length, chunk_size);
            length += chunk_size + 2;
            keys["body"] = keys["body"][length];
            chunk_size = strtol(keys["body"].c_str(), NULL, 16);
        }
        keys["body"] = decoded_body;
        keys["Transfer-Encoding"] = "";
        keys["Content-Length"] = length; // Later check if this is the right content size
    }

    std::cout << "Request body unchunked << " << keys["body"] << ">>" << std::endl;

    // Todo check if body is chunked

/*
    RFC pseudo code

    length := 0
    read chunk-size, chunk-ext (if any), and CRLF
    while (chunk-size > 0) {
        read chunk-data and CRLF
        append chunk-data to decoded-body
        length := length + chunk-size
        read chunk-size, chunk-ext (if any), and CRLF
    }
    read trailer field
    while (trailer field is not empty) {
    if (trailer field is allowed to be sent in a trailer) {
         append trailer field to existing header fields
    }
        read trailer-field
    }
    Content-Length := length
    Remove "chunked" from Transfer-Encoding
    Remove Trailer from existing header fields
*/

    //std::cout << "We've got a Request" << std::endl;
}



void Request::debug()
{
    // To reuse later on

    /*std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    while(it != keys.end())
    {
        std::cout << it->first << " :: " << it->second<<std::endl;
        it++;
    }*/
}
