#pragma once
#include "socket.hpp"
#include <vector>
#include <poll.h>
#include "./http/http_request.hpp"
#include "./http/http_response.hpp"
#include "../parcing/parcer.hpp"

#define LISTENER 0
#define REQUEST 1
#define RESPONSE 2

#define POLLBOTH (POLLIN | POLLOUT)

struct t_data
{
    int type;
    http_request request;
    http_response *response;
    std::string host;
    std::string port;
    size_t time;
};

class servers
{
    std::vector<struct pollfd> fd_poll;
    std::vector<t_data> data;
    std::vector<_socket> sockets;

    public:
        servers();
        void client_req_handler(size_t &index);
        void client_res_handler(size_t &index);
        void listener_handler(size_t &index);
        void delete_client(size_t index);
        int deploy();
};

void server_init(std::string confPath);