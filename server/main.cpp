#include "servers.hpp"

int main()
{
    std::string hosts[3] = {"127.0.0.1","127.0.0.1","0.0.0.0"};
    std::string ports[3] = {"8080","8082","8000"};

    servers raa9(hosts,ports,3);
    raa9.deploy();
}