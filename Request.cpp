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

    pos = line.find(' ', prev);
    keys["reqtype"] = line.substr(0, pos - prev);
    prev = pos + 1;

    pos = line.find(' ', prev);
    keys["path"] = line.substr(prev, pos - prev);
    prev = pos + 1;

    pos = line.find(' ', prev);
    keys["vers"] = line.substr(prev);

    std::cout << "Received a <<" << keys["reqtype"] << ">> request, to path <<" << keys["path"] << ">> on http version <<" << keys["vers"] << ">>" << std::endl;
    

    // To reuse later for debugging
    /*write(new_socket , hello.c_str() , hello.length());
    
    write(new_socket , "Received request type: ", strlen("Received request type: "));
    write(new_socket , reqtype.c_str() , reqtype.length());

    write(new_socket , "</br>path: ", strlen("</br>path: "));
    write(new_socket , path.c_str() , path.length());
    
    write(new_socket , "</br>Version: ", strlen("</br>Version: "));
    write(new_socket , vers.c_str() , vers.length());*/


    while (std::getline(to_stream, line))
    {
        line.resize(line.size() - 1);
        pos = line.find(':');
        if (pos != -1)
        {
            std::cout << line.substr(0, pos) << " :: " << line.substr(pos + 2) << std::endl; 
            keys[line.substr(0, pos)] = line.substr(pos + 2);
        }
    }
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
