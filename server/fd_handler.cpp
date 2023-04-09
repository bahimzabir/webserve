#include "servers.hpp"


int count_nl (char *str,int len)
{
    int nl = 0;
    int i = 0;
    while (i < len)
    {
        nl += ((str[i]) == '\n');
        i++;
    }
    return nl;
}

void servers::client_req_handler(int &index)
{
    if (!(fd_poll[index].revents & POLLIN))
        return ;
    std::cout << "REQUEST" << std::endl;//error in cout
    char buffer[1025];
    int ret;
    ret = recv(fd_poll[index].fd,buffer,1024,0);
    if (ret == -1)
        throw END;
    buffer[ret] = 0;
    data[index].request.parse_remaining(buffer,ret,count_nl(buffer,ret));
    if (data[index].request.get_state() == REQUEST_BODY)
    {
        fd_poll[index].events = POLLOUT;
        data[index].type = RESPONSE;
        data[index].response = new http_response(&data[index].request,&(fd_poll[index]),data[index].host,data[index].port);
        return;
    }
}

void servers::client_res_handler(int &index)
{
    if (!((fd_poll[index].events & POLLOUT) && (fd_poll[index].revents & POLLOUT)) && !((fd_poll[index].events & POLLIN) && (fd_poll[index].revents & POLLIN)))
        return ;
    data[index].response->generate_response(&(fd_poll[index]),&(data[index].request));
}

void servers::listener_handler(int &index)
{
    if (!(fd_poll[index].revents & POLLIN))
        return ;
    std::cout << "LISTENER" << std::endl;
    pollfd *p;
    t_data *d;

    fd_poll.push_back(pollfd());
    data.push_back(t_data());

    d = &data[data.size() - 1];
    p = &fd_poll[fd_poll.size() - 1];
    d->type = REQUEST;
    p->fd = accept(fd_poll[index].fd,NULL,NULL);
    if (p->fd == -1)
    {
        fd_poll.erase(fd_poll.end() - 1);
        data.erase(data.end() - 1);
    }
    else
    {
        p->events = POLLIN;
        p->revents = 0;
        d->host = data[index].host;
        d->port = data[index].port;
    }
}