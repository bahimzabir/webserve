#pragma once
#include "http_request.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <fstream>

#define HEADERS 1
#define OPEN_STREAM 2
#define BODY 3
#define ERROR 4
#define END 5





class http_response
{
    http_request &request;
    std::string status[3];
    std::map<std::string,std::string> headers;
    std::map<std::string,std::string> content_type;
    std::string res_header;
    std::string body;
    std::fstream file;
    int client_fd;
    int type;
    int state;
    public:
        http_response(http_request &req,int fd);
        void GET_handler();
        void POST_handler();
        void DELETE_handler();
        void SEND_handler();
        void ERROR_handler();
        void generate_response();
        void add_headers();
};



