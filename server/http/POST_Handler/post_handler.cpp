#include "../http_response.hpp"


int new_line(std::string &body)
{
    int i = 0;
    if (i < body.size() && body[i] == '\r')
        i++;
    if (i < body.size() && body[i] == '\n')
        i++;
    while(i < body.size())
    {
        if (body[i] == '\n')
            return 1;
        i++;
    }
    return 0;
}

void remove_white_spaces(std::string &body)
{

    if (body.size() && body[0] == '\r')
        body.erase(0,1);
    if (body.size() && body[0] == '\n')
        body.erase(0,1);
}

std::string extention(std::string &file)
{
    int pos = file.find_last_of('.');
    if (pos == std::string::npos)
        return "";
    return file.substr(pos + 1);
}

bool fexists(const char *filename)
{
    std::ifstream ifile(filename);
    if (ifile)
        return 1;
    else
        return 0;
}

void http_response::POST_upload_chunked_handler(void)
{
    char buffer[2048];
    int ret = 0;
    int amount = 0;
    std::string substring;
    if (content_remaining != -1 && (body == "" || !new_line(body)))
    { 
        if ((client->events & POLLIN) && (client->revents & POLLIN))
        {
            ret = recv(client->fd,buffer,2048,0);
            *timeout = get_time();
        }
        if (ret == -1 || !ret)
            throw END;
        body.append(buffer,ret);
    }
    while (1)
    {
        if (body == "" || (!content_remaining && !new_line(body)))
            break;
        if (!content_remaining)
        {
            while (!content_remaining && new_line(body))
            {
                remove_white_spaces(body);
                substring = body.substr(0,body.find('\n'));
                content_remaining = strtoll(substring.c_str(),NULL,16);
                conf.client_max_body_size -= content_remaining;
                if (conf.client_max_body_size <= 0)
                    throw ENTITY_LARGE;
                if (errno == ERANGE)
                    throw BAD_REQUEST;
                if (!content_remaining)
                    throw CREATED;
                if (content_remaining < 0)
                    throw BAD_REQUEST;
                body.erase(0,substring.size());
                remove_white_spaces(body);
                amount = (body.size() > content_remaining) ? content_remaining : body.size();
                substring = body.substr(0,amount);
                content_remaining -= amount;
                body.erase(0,amount);
                file.write(substring.c_str(),amount);
                if (!file.good())
                    throw SERVER_ERROR;
            }
        }
        else if (content_remaining)
        {
            amount = (body.size() > content_remaining) ? content_remaining : body.size();
            substring = body.substr(0,amount);
            content_remaining -= amount;
            body.erase(0,amount);
            file.write(substring.c_str(),amount);
            if (!file.good())
                throw SERVER_ERROR;
        }
    }
}

void http_response::POST_upload_normal_handler(void)
{
    
    char buffer[2048];
    int ret = 0;
    int size = 0;
    if (content_remaining != -1 && body.size() == 0)
    {
        ret = recv(client->fd,buffer,2048,0);
        *timeout = get_time();
    }
    if (ret == -1)
        throw END;
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
        throw SERVER_ERROR;
    if (!content_remaining)
        throw CREATED;
}



void http_response::POST_check_cgi()
{
    struct stat s;
    if (stat(conf.root.c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
        {
            for (int i = 0;i < conf.index.size();i++)
            {
                if (check_cgi(conf.index[i]))
                {
                    std::ifstream f(conf.root + "/" + conf.index[i]);
                    if (f.good())
                    {
                        is_cgi = 1;
                        conf.upload_pass = "/tmp";
                        conf.root = conf.root + "/" + conf.index[i];
                        f.close();
                        return;
                    }
                    else if (f.is_open())
                        f.close();
                }
            }
        }
        else
        {
            if (check_cgi(conf.root))
            {
                std::ifstream f(conf.root);
                if (f.good())
                {
                    is_cgi = 1;
                    conf.upload_pass = "/tmp";
                    f.close();
                    return;
                }
                else if (f.is_open())
                    f.close();
            }
        }
    }
}

void http_response::POST_check_state()
{
    //UPLOAD

    client->events = POLLIN;
    char buffer[5000];
    int ret = 0;
    request->get_remaining().read(buffer,5000);
    ret = request->get_remaining().gcount();
    state = CHUNKED;
    if (request->get_header("TRANSFER-ENCODING") == "chunked")
        content_remaining = 0;
    else
    {
        content_remaining = strtoll(request->get_header("CONTENT-LENGTH").c_str(),NULL,10);
        state = NORMAL;
        if (content_remaining > conf.client_max_body_size)
            throw ENTITY_LARGE;
        if (content_remaining <= ret)
        {
            content_remaining = -1;
            client->events = POLLOUT;
        }
    }
    
	if (conf.upload_pass == "")
        POST_check_cgi();
    if (conf.upload_pass != "")
    {
        std::string name;
        if (!is_cgi && request->get_header("FILE_NAME") != "")
        {
            name = conf.upload_pass + '/' + request->get_header("FILE_NAME");
            if (fexists(name.c_str()))
                throw FORBIDDEN;
        }
        else
            name = conf.upload_pass + "/XXXXXX";
        int fd = mkstemp(&name[0]);
        cgi_data.input_fd = -1;
        if (fd == -1)
            throw SERVER_ERROR;
        cgi_data.input = name;
        if (is_cgi)
        {
            cgi_data.input_fd = fd;
            std::string output = conf.upload_pass + "/XXXXXX";
            int ot_fd = mkstemp(&output[0]);
            if (ot_fd == -1)
                throw SERVER_ERROR;
            cgi_data.output = output;
            cgi_data.output_fd = ot_fd;
        }
        else
            close(fd);
        file.open(name);
        if (!file.good())
            throw FORBIDDEN;
        body.append(buffer,ret);
        std::cout << body << std::endl;
        client->revents = 0;
        if (state == CHUNKED)
            POST_upload_chunked_handler();
        return;
    }
    //CGI
    throw FORBIDDEN;
}

void http_response::POST_handler()
{
    if (state == CHUNKED)
        POST_upload_chunked_handler();
    else if (state == NORMAL)
        POST_upload_normal_handler();
}