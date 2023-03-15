#include "servers.hpp"

int main()
{
    try {
        config deflt = getServersInfos("parcing/default.conf")[0];
        std::vector<config> config_info = getServersInfos("parcing/config.conf");
        for (std::vector<config>::iterator it = config_info.begin(); it != config_info.end(); it++)
        {
            add_default_params(*it, deflt);
        }
        std::cout << "port = " << config_info[0].ports[0] << "\n";
        std::cout << "host = " << config_info[0].host << "\n";
        std::cout << "server_name = " << config_info[0].server_names[0] << "\n";
        std::cout << "rout_name = " << config_info[0].routes[0].route_name << "\n";
        std::cout << "rout_index = " << config_info[0].routes[0].index[0] << "\n";

    } catch (std::exception& obj) {
        std::cout << obj.what();
    }
    //servers raa9(config_info);
    //raa9.deploy();
}