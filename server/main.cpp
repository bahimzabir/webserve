#include "http/config_matcher.hpp"
#include "servers.hpp"

int main()
{
    config_match cf_match;

    try {
        server_init();
    } catch (std::exception& obj) {
        std::cout <<"server init failed: " <<obj.what();
        return(1);
    }
    // std::cout << "port = " << config_info[0].ports[0] << "\n";
    // std::cout << "host = " << config_info[0].host << "\n";
    // std::cout << "server_name = " << config_info[0].server_names[0] << "\n";
    // std::cout << "rout_name = " << config_info[0].routes[0].route_name << "\n";
    // std::cout << "rout_index = " << config_info[0].routes[0].index[0] << "\n";
    // std::cout << "cgi_pass = " << config_info[0].routes[1].cgi_pass[0].cgi_param << "\n";
    // std::cout << "cmbs = " << config_info[0].client_max_body_size << "\n";

    cf_match = get_config("127.0.0.1", "9000", "/", "server_2");

    std::cout << cf_match.err_pages[400] << "\n";
    std::cout << cf_match.methods[0] << "\n";
    std::cout <<cf_match.root << "\n";
    std::cout << cf_match.index[0] << "\n";
    std::cout << cf_match.autoindex << "\n";
    std::cout << cf_match.upload_pass << "\n";
    std::cout << cf_match.client_max_body_size << "\n";
    std::cout << cf_match.return_value << "\n";
    // servers raa9;
    // raa9.deploy();
}