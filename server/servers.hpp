#pragma once
#include "socket.hpp"
#include <vector>
#include <poll.h>
#include "../http/http_request.hpp"
#include "../http/http_response_generator.hpp"

#define LISTENER 0
#define REQUEST 1
#define RESPONSE 2
#define FILE 3
#define UPLOAD 4
#define CGI 5

#define POLLBOTH (POLLIN | POLLOUT)


struct t_data
{
    int type;
    http_request request;
    http_response *response;
};



class servers
{
    std::vector<struct pollfd> fd_poll;
    std::vector<t_data> data;
    std::vector<_socket> sockets;
    
    //std::vector<servers> configs;
    public:
        servers(const std::string *host,const std::string *ports,int len);
        void client_req_handler(int &index);
        void client_res_handler(int &index);
        void listener_handler(int &index);
        void file_handler(int index);
        int deploy();
};