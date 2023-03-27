#include "../http_response.hpp"


void http_response::POST_upload_chunked_handler(void)
{
    char buffer[2048];
    int ret = 0;
    if (content_remaining != -1)
        ret = recv(client->fd,buffer,2048,0);
    body.append(buffer,ret);
    
}

void http_response::POST_upload_normal_handler(void)
{
    
    char buffer[2048];
    int ret = 0;
    int size = 0;
    if (content_remaining != -1)
        ret = recv(client->fd,buffer,2048,0);
    if (ret == -1)
        throw 500;
    if (content_remaining == -1)
        content_remaining = body.size();
    body.append(buffer,ret);
    size = body.size();
    if (body.size() > content_remaining)
        size = content_remaining;
    file.write(body.c_str(),size);
    body.erase(0,size);
    content_remaining -= size;
    if (!file.good())
        throw 500;
    if (!content_remaining)
        throw 201;
}


void http_response::POST_check_state()
{
    client->events = POLLIN;
    char buffer[5000];
    int ret = 0;
    request.get_remaining().read(buffer,5000);
    ret = request.get_remaining().gcount();
    std::string rem(buffer,ret);

    if (content_remaining < 0)
        throw 400;
    if (request.get_header("TRANSFER-ENCODING") == "chunked")
        state = CHUNKED;
    else
        state = NORMAL;
    content_remaining = std::stoll(request.get_header("CONTENT-LENGTH"));
    if (request.get_header("TRANSFER-ENCODING") == "chunked" && rem.find("\n0\n") != std::string::npos)
    {
        content_remaining = -1;
        client->events = POLLOUT;
    }
    if (std::stoll(request.get_header("CONTENT-LENGTH")) <= rem.size())
    {
        content_remaining = -1;
        client->events = POLLOUT;
    }
    conf.upload_pass = "/Users/hait-moh/Desktop/webserv/webserve";

    if (conf.upload_pass != "")
    {
        std::string name = request.get_method();
        int name_start = name.find_last_of('/');
        if (name_start == std::string::npos || name.substr(name_start) == "")
        {
            name = conf.upload_pass + "/XXXXXX";
            int fd = mkstemp(&name[0]);
            if (fd == -1)
                throw 500;
            close(fd);
        }
        else
            name = name.substr(name_start);
        std::cout << name << std::endl;
        file.open(name);
        if (!file.good())
        {
            std::cout << "mablanch" << std::endl;
            throw 403;
        }
        body += rem;
        return;
    }
    for (int i =0; i < conf.cgi_pass.size();i++)
    {
        std::cout << conf.cgi_pass[i].cgi_param << std::endl;
    }
}

void http_response::POST_handler()
{
    if (state == CHUNKED)
        POST_upload_chunked_handler();
    else if (state == NORMAL)
        POST_upload_normal_handler();
}