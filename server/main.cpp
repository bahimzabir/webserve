#include "servers.hpp"

int main()
{
    std::vector<config> config_info = getServersInfos("parcing/config.conf");
    servers raa9(config_info);
    raa9.deploy();
}