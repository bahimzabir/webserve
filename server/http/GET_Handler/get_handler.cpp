#include "../http_response.hpp"

size_t getSize(std::string filename)
{
    struct stat results;
    if (stat(filename.c_str(),&results) == 0)
    {
        std::cout << "size :           ::::::::::" << results.st_size << std::endl;
        return results.st_size;
    }
    return 0;
}

void http_response::GET_check_state()
{
    client->events = POLLOUT;
    struct stat s;
    std::cout << conf.root << std::endl;
    if (stat(conf.root.c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
        {
            if (request->get_path().back() != '/')
            {
                headers["LOCATION"] = (request->get_path() + "/");
                std::cout << "************" << headers["LOCATION"] <<std::endl;
                throw 301;
            }
            for (int i = 0;i < conf.index.size();i++)
            {
                std::ifstream f(conf.root + "/" + conf.index[i]);
                if (f.good())
                {
                    state = FILE;
                    conf.root = conf.root + "/" + conf.index[i];
                    if (check_cgi(conf.index[i]))
                        type = CGI;
                    else
                        type = GET;
                    f.close();
                    return;
                }
                else if (f.is_open())
                    f.close();
            }
            if (conf.autoindex)
                state = LIST_DIRECTORY;
            else
                throw FORBIDDEN;
        }
        else
        {
            if (check_cgi(conf.root))
                type = CGI;
            else
            {
                type = GET;
                state = FILE;
            }
        }
    }
    else
        throw NOT_FOUND;
}


void http_response::GET_open_input()
{
    file.open(conf.root);
    std::cout << conf.root << std::endl;
    if (!file)
        throw FORBIDDEN;
    res_header += "HTTP/1.1 200 OK\n";
    content_remaining = getSize(conf.root);
    headers["Content-Length"] = int_to_string(content_remaining);
    std::string ext = (conf.root).substr(conf.root.find_last_of(".") + 1);
    if (ext != conf.root && content_type.count(ext))
        headers["Content-Type"] = content_type[ext];
    else
        headers["Content-Type"] = "application/octet-stream";
    for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        res_header +=  (*it).first + ":" + (*it).second + "\n";
    res_header += "\n";
    state = RESPONSE_BODY;
}
void http_response::GET_body()
{
    char buffer[10001];
    if (body == "" && res_header == "")
    {
        file.read(buffer,10000);
        buffer[file.gcount()] = 0;
        body.append(buffer,file.gcount());
    }
    SEND_handler();
    if (!file.good() && !file.eof() && body == "" && res_header == "")
    {
        std::cout << "error hna" << std::endl;
        throw 666;
    }
}


void http_response::GET_list_directory()
{
    DIR *directory;
    struct dirent *d;
    directory = opendir(conf.root.c_str());
    if (!directory)
        throw FORBIDDEN;
    d = readdir(directory);
    while (d)
    {
        std::string path = d->d_name;
        body += "<a href='" + path + "'>" + path + "</a><br /><br />\n";
        d = readdir(directory);
    }
    content_remaining = body.size();
    headers["Content-Length"] = int_to_string(body.size());
    headers["Content-Type"] = content_type["html"];
    res_header += "HTTP/1.1 200 OK\n";
    for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        res_header +=  (*it).first + ":" + (*it).second + "\n";
    res_header += "\n";
    type = SEND;
}

void http_response::GET_handler()
{
    std::cout << "GET_H" << std::endl;
    if (state == FILE)
        GET_open_input();
    else if (state == LIST_DIRECTORY)
        GET_list_directory();
    else if (state == RESPONSE_BODY)
        GET_body();
}