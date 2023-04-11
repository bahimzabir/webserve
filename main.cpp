#include "server/http/config_matcher.hpp"
#include "server/servers.hpp"

int main(int arc, char **arv)
{
    config_match cf_match;
    std::string confPath;

    if (arc > 2) {
        std::cerr << "Usage: ./webserve [config file path]\n";
        exit(1);
    }
    confPath = (arc == 2) ? arv[1] : DEF_COF_PATH;
    try {
        server_init(confPath);
    } catch (std::exception& obj) {
        std::cout <<"server init failed: " <<obj.what();
        return(1);
    }
    servers webserve;
    webserve.deploy();
}