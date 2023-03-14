#include "servers.hpp"

servers::servers(std::vector<config> &config_info)
{
    pollfd p;
    t_data d;
    d.type = LISTENER;
    for (int i = 0;i < config_info.size();i++)
    {
        for (int port_index = 0; port_index < config_info[i].ports.size();port_index++)
        {
            try
            {   
                sockets.push_back(_socket(config_info[i].host,config_info[i].ports[i]));
                d.conf = &(config_info[i]);
                data.push_back(d);
                p.fd = sockets[i].get_socket_fd();
                
                p.events = POLLIN;
                p.revents = 0;
                fd_poll.push_back(p);
            }
            catch(int x)
            {
                if (x < 0)
                    close(x);
                std::cout << "error" << std::endl;
            }
        }
    }
}
int servers::deploy()
{
    void (servers::*handlers[4])(int &) = {&servers::listener_handler,&servers::client_req_handler,&servers::client_res_handler};
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