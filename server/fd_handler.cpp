#include "servers.hpp"

void servers::client_req_handler(int &index)
{
    if (!(fd_poll[index].revents & POLLIN))
        return ;
    std::cout << "REQUEST" << std::endl;//error in cout
    char buffer[1000];
    int ret;
    ret = recv(fd_poll[index].fd,buffer,1000,0);
    if (ret == -1)
        throw "client error";
    buffer[ret] = 0;
    data[index].request.req += buffer;
    if (ret == 0 || ret < 1000)
    {
        http_parser(data[index].request);
        fd_poll[index].events = POLLOUT;
        data[index].type = RESPONSE;
        data[index].response = new http_response(data[index].request,fd_poll[index].fd);
        return;
    }
    std::cout << "blan\n" << std::endl;
}

void servers::client_res_handler(int &index)
{
    if (!(fd_poll[index].revents & POLLOUT))
        return ;
    std::cout << "RESPONSE" << std::endl;
    try
    {
        std::cout << "HNA   " << std::endl;
        data[index].response->generate_response();
    }
    catch(int status)
    {
        if (status == 200)
        {
            close(fd_poll[index].fd);
            fd_poll.erase(fd_poll.begin() + index);
            delete data[index].response;
            data.erase(data.begin() + index);
            index--;
        }
    }
    
    
}
void servers::listener_handler(int &index)
{
    
    if (!(fd_poll[index].revents & POLLIN))
        return ;
    std::cout << "LISTENER" << std::endl;
    pollfd p;
    t_data d;

    d.type = REQUEST;
    p.fd = accept(fd_poll[index].fd,NULL,NULL);
    if (p.fd == -1)
        std::cout << "error" << std::endl;
    else
    {
        p.events = POLLIN;
        p.revents = 0;
        fd_poll.push_back(p);
        data.push_back(d);
    }
}
void servers::file_handler(int index)
{

}