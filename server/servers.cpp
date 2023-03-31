#include "servers.hpp"

servers::servers()
{
    pollfd p;
    t_data d;
    d.type = LISTENER;
    int index = 0;

    for (int i = 0;i < config_info.size();i++)
    {
        for (int port_index = 0; port_index < config_info[i].ports.size();port_index++)
        {
            try
            {   
                sockets.push_back(_socket(config_info[i].host,config_info[i].ports[port_index]));
                d.host = config_info[i].host;
                d.port = config_info[i].ports[port_index];
                d.response = NULL;
                data.push_back(d);
                p.fd = sockets[index].get_socket_fd();
                p.events = POLLIN;
                p.revents = 0;
                fd_poll.push_back(p);
                index++;
            }
            catch(int x)
            {
                if (x >= 0)
                    close(x);
            }
        }
    }
    if (!sockets.size())
    {
        std::cerr << "No server On" << std::endl;
        exit(0);
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
                try
                {
                    (this->*handlers[data[i].type])(i);//CALL THE function handler based on type
                }
                catch (int status)
                {
                    if (status == END)
                    {
                        close(fd_poll[i].fd);
                        fd_poll.erase(fd_poll.begin() + i);
                        delete data[i].response;
                        data.erase(data.begin() + i);
                        i--;
                    }
                }
                fd_poll[i].revents = 0;
                num_of_revents--;
            }
        }
    }
}