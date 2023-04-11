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
    long long time;
};

class servers
{
    std::vector<struct pollfd> fd_poll;
    std::vector<t_data> data;
    std::vector<_socket> sockets;

    public:
        servers();
        void client_req_handler(int &index);
        void client_res_handler(int &index);
        void listener_handler(int &index);
        void file_handler(int index);
        void delete_client(int index);
        int deploy();
        void timeout_handler(int index);
};

void server_init();