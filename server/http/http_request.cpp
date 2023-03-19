#include "http_request.hpp"


http_request::http_request()
{
    state  = HTTP_HEADER;
}
void http_request::push_header(std::string &line)
{
    std::pair<std::string,std::string> pair;
    std::string field;
    std::stringstream str_stream(line);


    getline(str_stream,field,':');
    pair.first = field;
    getline(str_stream,field,':');
    pair.second = field;
    headers.push_back(pair);
}

void http_request::http_header_handler()
{
    std::cout << "http_header" << std::endl;
    if (!remaining_nl)
        return;
    std::string line;
    getline(remaining,line,'\n');

    std::stringstream str_stream(line);
    std::string field;

    getline(str_stream,field,' ');
    http_header[0] = field;
    getline(str_stream,field,' ');
    http_header[1] = field;
    getline(str_stream,field,' ');
    http_header[2] = field;
    state = REQUEST_HEADERS;
}

void http_request::headers_handler()
{
    std::cout << "headers" << std::endl;
    std::string line;
    while (remaining_nl)
    {
        getline(remaining,line,'\n');
        if (line == "")
        {
            state = REQUEST_BODY;
            break;
        }
        push_header(line);
        remaining_nl--;
    }
    //handle body;
}

int http_request::get_state()
{
    return state;
}


void http_request::parse_remaining(char *buffer,int len, int n_new_line)
{
    remaining_nl += n_new_line;
    remaining.write(buffer,len);
    void (http_request::*handlers[2])() = {&http_request::http_header_handler,&http_request::headers_handler};
    std::cout << "dkhlat" << state << std::endl;
    (this->*handlers[state])();
}


http_request &http_request::operator=(const http_request &obj)
{
    state = obj.state;
    return *this;
}

http_request::http_request(const http_request &obj)
{
    state = obj.state;
    return;
}

const std::string &http_request::get_method()
{
    return http_header[0];
}
const std::string &http_request::get_path()
{
    return http_header[1];
}
const std::string &http_request::get_version()
{
    return http_header[2];
}