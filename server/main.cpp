#include "http/config_matcher.hpp"
#include "servers.hpp"
int main()
{
    config_match cf_match;

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
        //std::cout << "cgi_pass = " << config_info[0].routes[0].cgi_pass[0].cgi_param << "\n";
        std::cout << "cmbs = " << config_info[0].client_max_body_size << "\n";

         cf_match = get_config("127.0.0.1", "9000", "/", "server_2", config_info, deflt);
        	
        // std::cout << cf_match.methods[0] << "\n";
        // std::cout << "new Root " <<cf_match.root << "\n";
        // std::cout << cf_match.index[0] << "\n";
        //  std::cout << cf_match.autoindex << "\n";
        // std::cout << cf_match.upload_pass << "\n";
        // std::cout << cf_match.cgi_pass[0].cgi_param << "\n";
        // std::cout << cf_match.client_max_body_size << "\n";
        // std::cout << cf_match.err_pages_struct[0].error_file << "\n";
        // std::cout << cf_match.return_value << "\n";
        servers raa9(config_info);
        raa9.deploy();
    } catch (std::exception& obj) {
        std::cout <<"exception: " <<obj.what();
    }


}