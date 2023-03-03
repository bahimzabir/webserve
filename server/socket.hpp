#pragma once
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>

class _socket
{
    struct addrinfo *addr;
    int socket_fd;
    public:
        _socket(const std::string &host,const std::string &port);
        int get_socket_fd();
        struct addrinfo *get_info();
};