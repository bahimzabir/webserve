#include "../http_response.hpp"



void http_response::GET_open_input()
{
    std::string file_path = conf.root;
    std::cout << file_path << std::endl;
    file.open(file_path);
    if (!file)
        throw 403;
    state = RESPONSE_BODY;
}
void http_response::GET_body()
{
    char buffer[1001];
    file.read(buffer,1000);
    buffer[file.gcount()] = 0;
    body.append(buffer,file.gcount());
    std::cout << file.gcount() << std::endl;
    if (file.eof())//ended file generation
    {
        type = SEND;
        body += '\n';
        headers["Content-Length"] = int_to_string(body.size());
        std::string ext = (request.get_path()).substr(request.get_path().find_last_of(".") + 1);
        if (ext != request.get_path() && content_type.count(ext))
            headers["Content-Type"] = content_type[ext];
        else
            headers["Content-Type"] = "text/plain";
        res_header += "HTTP/1.1 200 OK\n";
        for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
            res_header +=  (*it).first + ":" + (*it).second + "\n";
        res_header += "\n";
        std::cout << res_header << "---------" << std::endl;
    }
    else if (!file.good())
    {
        std::cout << "fail" << std::endl;
        throw 666;
    }
}


void http_response::GET_list_directory()
{
    DIR *directory;
    struct dirent *d;
    directory = opendir(conf.root.c_str());
    if (!directory)
        throw 403;
    d = readdir(directory);
    while (d)
    {
        std::string path = d->d_name;
        body += "<a href='" + path + ">" + path + "</a><br /><br />\n";
        d = readdir(directory);
    }
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