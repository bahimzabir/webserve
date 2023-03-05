#include "servers.hpp"

void servers::client_req_handler(int index)
{
    char buffer[1000];
    int ret;
    ret = recv(fd_poll[index].fd,buffer,1000,0);
    if (ret == -1)
        throw "client error";
    if (ret == 0)
    {
        http_parser(data[index].request);
        fd_poll[index].events = POLLOUT;
        data[index].type = RESPONSE;
        return;
    }
    buffer[ret] = 0;
    data[index].request.req += buffer;

}
void servers::client_res_handler(int index)
{
    generate_response(data[index]);


    fd_poll[index].events = POLLIN;
}
void servers::listener_handler(int index)
{
    pollfd p;
    t_data d;

    d.type = REQUEST;
    p.fd = accept(fd_poll[index].fd,NULL,NULL);
    if (p.fd == -1)
        std::cout << "error" << std::endl;
    else
    {
        p.events = POLLIN;
        fd_poll.push_back(p);
        data.push_back(d);
    }
}
void servers::file_handler(int index)
{

}