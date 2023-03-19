#pragma once
#include "http_request.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include "../../parcing/parcer.hpp"
#include "config_matcher.hpp"
#include <dirent.h>
#include "error_pages_map.hpp"
#include <poll.h>


#define FILE 1
#define LIST_DIRECTORY 2
#define RESPONSE_BODY 3
#define UPLOADING 4
#define RESPONSE_ERROR 5
#define END 6



#define GET 0 
#define POST 1
#define DELETE 2
#define SEND 3
#define NOT_IMPLEMENTED 501
#define NOT_ALLOWED 405

class http_response
{
    http_request &request;
    std::string status[3];
    std::map<std::string,std::string> headers;
    std::map<std::string,std::string> content_type;
    std::string res_header;
    std::string body;
    std::fstream file;
    config_match conf;
    struct pollfd *client;
    int type;
    int state;



    void add_headers();
    void check_state();

    void GET_handler();
    void GET_open_input();
    void GET_body();
    void GET_list_directory();

    void POST_handler();

    void DELETE_handler();

    void SEND_handler();
    void ERROR_handler(int x);


    public:
        http_response(http_request &req,struct pollfd *fd);
        void generate_response();

};

std::string int_to_string(int a);

