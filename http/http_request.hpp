#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>  
#include <sstream>

struct http_request
{
    std::string http_header[3];
    std::vector<std::pair <std::string,std::string> > headers;
    std::string body;
    std::string req;
};

void http_parser(http_request &request);
