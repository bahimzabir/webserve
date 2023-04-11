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
                p.fd = sockets[index].get_socket_fd();
                p.events = POLLIN;
                p.revents = 0;
                data.push_back(d);
                fd_poll.push_back(p);
                index++;
            }
            catch(int x)
            {
                if (x >= 0)
                    close(x);
            }
            catch(std::exception &exception)
            {
                std::cout << exception.what() << std::endl;
                exit(1);
            }
        }
    }
    if (!sockets.size())
    {
        std::cerr << "No server On" << std::endl;
        exit(0);
    }
}
void servers::delete_client(int index)
{
    close(fd_poll[index].fd);
    fd_poll.erase(fd_poll.begin() + index);
    delete data[index].response;
    data.erase(data.begin() + index);
}

int servers::deploy()
{
    void (servers::*handlers[4])(int &) = {&servers::listener_handler,&servers::client_req_handler,&servers::client_res_handler};
    int num_of_revents;
    signal(SIGPIPE, SIG_IGN);
    while (1)
    {
        num_of_revents = poll(&(fd_poll[0]),fd_poll.size(),-1);//wait for events
        for (int i = 0;i < fd_poll.size();i++)
        {
            std::cout << data.size() << "-------------" << fd_poll.size() << std::endl;
            if (num_of_revents <= 0)
                break;
            try
            {
                if (fd_poll[i].revents & POLLBOTH)
                {
                    (this->*handlers[data[i].type])(i);//CALL THE function handler based on type
                    fd_poll[i].revents = 0;
                    num_of_revents--;
                }
                if (data[i].type != LISTENER && get_running_time(data[i].time) >= 3000)
                    throw END;
            }
            catch (int status)
            {
                if (status == END)
                    delete_client(i--);
            }
            catch (std::exception &Exception)
            {
                if (data[i].type == LISTENER && data.size() != fd_poll.size())
                    fd_poll.erase(fd_poll.end() - 1);
                else
                    delete_client(i--);
            } 
        }
    }
}