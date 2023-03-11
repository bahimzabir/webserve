#pragma once
#include "http_request.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <fstream>

#define OPEN_STREAM 1
#define RESPONSE_BODY 2
#define RESPONSE_ERROR 3
#define END 4



#define GET 0
#define POST 1
#define DELETE 2
#define SEND 3





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
    void add_headers();
    void GET_handler();
    void POST_handler();
    void DELETE_handler();
    void SEND_handler();
    void ERROR_handler();
    public:
        http_response(http_request &req,int fd);
        void generate_response();

};

std::string int_to_string(int a);

