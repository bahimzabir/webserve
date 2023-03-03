#pragma once
#include "socket.hpp"
#include <vector>
#include <poll.h>

#define listener 1
#define client 2
#define POLLBOTH (POLLIN | POLLOUT)




class servers
{
    std::vector<struct pollfd> fd_poll;
    std::vector<int> type;
    std::vector<_socket> sockets;
    public:
        servers(const std::string *host,const std::string *ports,int len);
        int deploy();
};