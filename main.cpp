#include "Config.hpp"



int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./Webserv [config_file]" << std::endl;
        return 1;
    }
    try
    {
        Config config((std::string(argv[1])));
        config.debug();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}