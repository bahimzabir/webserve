#include "http_response.hpp"
struct config_match
{
    std::vector<std::string>	methods;
    std::string                 root;
    std::vector<std::string>	index;
    std::string					autoindex;
    std::string					upload_pass;
    std::vector<cgi>			cgi_pass;
    std::string					client_max_body_size;
	std::vector<errorPage>		error_pages;
};