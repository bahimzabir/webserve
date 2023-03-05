#pragma once
#include "socket.hpp"
#include <vector>
#include <poll.h>
#include "../http/http_request.hpp"

#define LISTENER 1
#define REQUEST 2
#define RESPONSE 3
#define FILE 4
#define UPLOAD 5
#define CGI 6

#define POLLBOTH (POLLIN | POLLOUT)


struct t_data
{
    int type;
    http_request request;
    std::string response;
};



class servers
{
    std::vector<struct pollfd> fd_poll;
    std::vector<t_data> data;
    std::vector<_socket> sockets;
    
    //std::vector<servers> configs;
    public:
        servers(const std::string *host,const std::string *ports,int len);
        void client_req_handler(int index);
        void client_res_handler(int index);
        void listener_handler(int index);
        void file_handler(int index);
        int deploy();
};