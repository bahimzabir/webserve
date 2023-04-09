#include "socket.hpp"


void close_Throw_error(const std::string &function_err,int fd,const std::string &host,const std::string &port,addrinfo *addr)
{
    std::cout << function_err << "    " <<  host << ":"  << port << "   ->    " << strerror(errno) << std::endl;
    freeaddrinfo(addr);
    throw fd;
}


_socket::_socket(const std::string &host,const std::string &port)
{
    struct addrinfo hints;
    int on = 1;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int err;
    std::cout << host << ":" << port << std::endl;
    err = getaddrinfo(host.c_str(),port.c_str(),&hints,&addr);
    if (err != 0)
    {
        std::cout << gai_strerror(err) << std::endl;
        throw -1;
    }
    socket_fd = socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if (socket_fd == -1)
        close_Throw_error("socket:",socket_fd,host,port,addr);
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
        close_Throw_error("fcntl:",socket_fd,host,port,addr);
    if (setsockopt(socket_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        close_Throw_error("setsocketopt:",socket_fd,host,port,addr);
    if (bind(socket_fd,addr->ai_addr,addr->ai_addrlen) == -1)
        close_Throw_error("bind:",socket_fd,host,port,addr);
    if (listen(socket_fd,100) == -1)
        close_Throw_error("listen:",socket_fd,host,port,addr);
}
int _socket::get_socket_fd()
{
    return socket_fd;
}