#pragma once
#include "http_response.hpp"
#include "../servers.hpp"
#include <algorithm>

struct config_match
{
    // TO DO: autoindex & mbs to int 
    std::vector<std::string>	methods;
    std::string                 root;
    std::vector<std::string>	index;
    std::string					autoindex;
    std::string					upload_pass;
    std::vector<cgi>			cgi_pass;
    std::string					client_max_body_size;
	std::vector<errorPage>		error_pages;
    std::string                 return_value;
};

//Note: you have to free the config_match returned after using it
config_match& get_config(std::string host, std::string port, std::string rout, std::string server_name, std::vector<config>& config_info, config& def);


//(ip,port,directory,domain-name)