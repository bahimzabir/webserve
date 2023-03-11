#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>  
#include <sstream>
#include <sys/socket.h>

#define HTTP_HEADER 0
#define REQUEST_HEADERS 1
#define REQUEST_BODY 2


class http_request
{
    std::string http_header[3];
    std::vector<std::pair <std::string,std::string> > headers;
    std::stringstream remaining;
    int state;
    int remaining_nl;
    void http_header_handler();
    void headers_handler();
    void body_handler();
    void push_header(std::string &line);


    public:
        http_request();
        http_request &operator=(const http_request &obj);
        http_request(const http_request &obj);
        void parse_remaining(char *buffer,int len, int n_new_line);
        const std::string &get_method();
        const std::string &get_version();
        const std::string &get_path();
};