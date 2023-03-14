#include "../http_response.hpp"

void http_response::GET_open_input()
{
    std::string file_path = "/Users/hait-moh/Desktop/webserv/webserve" + request.get_path();
    std::cout << file_path << std::endl;
    file.open(file_path);
    std::cout << file.good() << std::endl;
    if (!file.good())
        throw 200;
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
        throw 200;
    }
}


void http_response::GET_open_directory()
{
    
}

void http_response::GET_handler()
{
    std::cout << "GET_H" << std::endl;
    if (state == FILE)
        GET_open_input();
    else if (state == DIRECTORY)
        GET_open_directory();
    else if (state == RESPONSE_BODY)
        GET_body();
}