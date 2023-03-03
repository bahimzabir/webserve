#include "servers.hpp"

servers::servers(const std::string *host,const std::string *ports,int len)
{
    pollfd p;
    for (int i = 0;i < len;i++)
    {
        try
        {
            sockets.push_back(_socket(host[i],ports[i]));
            p.fd = sockets[i].get_socket_fd();
            p.events = POLLIN;
            p.revents = 0;
            fd_poll.push_back(p);
            type.push_back(listener);
        }
        catch(int x)
        {
            if (x < 0)
                close(x);
            std::cout << "error" << std::endl;
        }
    }
    std::cout << "khrjat" << std::endl;
}
int servers::deploy()
{
    struct pollfd p;
    p.events = POLLIN;
    p.revents = 0;
    char buffer[6000];
    int num_of_revents;

    while (1)
    {
        num_of_revents = poll(&(fd_poll[0]),fd_poll.size(),-1);
        for (int i = 0;i < fd_poll.size();i++)
        {
            if (num_of_revents <= 0)
                break;
            if (fd_poll[i].revents & POLLBOTH)
            {
                num_of_revents--;
                if (type[i] == listener && (fd_poll[i].revents & POLLIN))
                {
                    p.fd = accept(fd_poll[i].fd,NULL,NULL);
                    if (p.fd == -1)
                        std::cout << "error" << std::endl;
                    else
                    {
                        fd_poll.push_back(p);
                        type.push_back(client);
                    }
                }
                else if (type[i] == client)
                {
                    if (fd_poll[i].revents & POLLIN)
                    {
                        buffer[recv(fd_poll[i].fd,buffer,6000,0)] = 0;
                        std::cout << "aydalalay ayt tmazirti" << std::endl;
                        fd_poll[i].events = POLLOUT;
                    }
                    else if (fd_poll[i].revents & POLLOUT)
                    {
                        std::cout << "aydalalay" << std::endl;
                        send(fd_poll[i].fd,"lfinitio",strlen("lfinitio"),0);
                        close(fd_poll[i].fd);
                        type.erase(type.begin() + i);
                        fd_poll.erase(fd_poll.begin() + i);
                        i--;
                    }
                    
                }
                fd_poll[i].revents = 0;
            }
        }
    }
}