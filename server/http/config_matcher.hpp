#pragma once
#include "../../parcing/parcer.hpp"
#include <algorithm>

struct config_match
{
    // TO DO: autoindex & mbs to int 
    std::vector<std::string>	methods;
    std::string                 root;
    std::vector<std::string>	index;
    int     					autoindex;
    std::string					upload_pass;
    std::vector<cgi>			cgi_pass;
    long				    	client_max_body_size;
	std::map<int, std::string>	err_pages;
    std::string                 return_value;

    ~config_match() {
        delete this;
    };
};

//Note: you have to free the config_match returned after using it
config_match& get_config(std::string host, std::string port, std::string rout, std::string server_name);


//(ip,port,directory,domain-name)