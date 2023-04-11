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
    void push_header(std::string &line);
    size_t *timeout;
    public:
        http_request();
        http_request &operator=(const http_request &obj);
        http_request(const http_request &obj);
        void parse_remaining(char *buffer,int len, int n_new_line,size_t *out_time);
        const std::string &get_method();
        const std::string &get_version();
        std::string &get_path();
        int get_state();
        std::string get_header(const std::string &header);
        std::string get_headers();
        std::stringstream &get_remaining();
        void reset();
        void isValidHttpRequest();
};
int count_nl (char *str,int len);
