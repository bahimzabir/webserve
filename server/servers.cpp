#include "servers.hpp"

servers::servers(const std::string *hosts,const std::string *ports,int len)
{
    pollfd p;
    for (int i = 0;i < len;i++)
    {
        try
        {
            data.push_back(t_data());
            sockets.push_back(_socket(hosts[i],ports[i]));
            p.fd = sockets[i].get_socket_fd();
            p.events = POLLIN;
            p.revents = 0;
            fd_poll.push_back(p);
            data[i].type = REQUEST;
        }
        catch(int x)
        {
            if (x < 0)
                close(x);
            std::cout << "error" << std::endl;
        }
    }
}
int servers::deploy()
{
    struct pollfd p;
    void (servers::*handlers[4])(int) = {&servers::listener_handler,&servers::client_req_handler,&servers::client_res_handler};
    p.events = POLLIN;
    p.revents = 0;
    char buffer[1000];
    int num_of_revents;

    while (1)
    {
        num_of_revents = poll(&(fd_poll[0]),fd_poll.size(),-1);//wait for events
        for (int i = 0;i < fd_poll.size();i++)
        {
            if (num_of_revents <= 0)
                break;
            if (fd_poll[i].revents & POLLBOTH)
            {
                (this->*handlers[data[i].type])(i);//CALL THE function handler based on type
                num_of_revents--;
            }
        }
    }
}