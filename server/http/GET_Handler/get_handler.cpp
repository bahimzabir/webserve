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
    conf.root = "/Users/hait-moh/Desktop/webserv/webserve" + request.get_path();
    std::cout << conf.root << std::endl;
    if (stat(conf.root.c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
        {
            for (int i = 0;i < conf.index.size();i++)
            {
                std::ifstream f(conf.root + "/" + conf.index[i]);
                std::cout << "------ ::::: " << conf.root + conf.index[i] << std::endl; 
                if (f.good())
                {
                    state = FILE;
                    conf.root = conf.root + conf.index[i];
                    if (check_cgi(conf.index[i]))
                        type = CGI;
                    else
                        type = GET;
                    f.close();
                    break;
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
                state = FILE;
        }
    }
    else
        throw NOT_FOUND;
    if (type == CGI)
    {
        std::string output = "/tmp/XXXXXX";
        int fd = mkstemp(&output[0]);
        if (fd == -1)
            throw SERVER_ERROR;
        cgi_data.output = output;
        cgi_data.output_fd = fd;
    }
}


void http_response::GET_open_input()
{
    std::string file_path = "/Users/hait-moh/Desktop/webserv/webserve" + request.get_path();
    std::cout << file_path << std::endl;
    file.open(file_path);
    if (!file)
        throw FORBIDDEN;
    res_header += "HTTP/1.1 200 OK\n";
    content_remaining = getSize(file_path);
    headers["Content-Length"] = int_to_string(content_remaining);
    std::string ext = (conf.root).substr(conf.root.find_last_of(".") + 1);
    if (ext != conf.root && content_type.count(ext))
        headers["Content-Type"] = content_type[ext];
    else
        headers["Content-Type"] = "application/octet-stream";
    for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        res_header +=  (*it).first + ":" + (*it).second + "\n";
    res_header += "\n";
    std::cout << "bdat asaaaat+++" << std::endl;
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
        std::cout << file.gcount() << std::endl;
    }
    SEND_handler();
    if (!file.good() && !file.eof())
    {
        std::cout << body << std::endl;
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