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


    std::getline(str_stream,field,':');
    for (int i = 0; i < field.size();i++)
        field[i] = toupper(field[i]);
    pair.first = field;
    
    std::getline(str_stream,field,':');
    for (int i = 0; i < field.size();i++)
        field[i] = toupper(field[i]);
    pair.second = field;
    headers.push_back(pair);
}

void http_request::http_header_handler()
{
    std::cout << "http_header" << std::endl;
    if (!remaining_nl)
        return;
    std::string line;
    std::getline(remaining,line,'\n');

    std::stringstream str_stream(line);
    std::string field;

    std::getline(str_stream,field,' ');
    http_header[0] = field;
    std::getline(str_stream,field,' ');
    http_header[1] = field;
    std::getline(str_stream,field,' ');
    http_header[2] = field;
    state = REQUEST_HEADERS;
    remaining_nl--;
}

void http_request::headers_handler()
{
    std::cout << "headers" << std::endl;
    std::string line;
    while (remaining_nl)
    {
        std::getline(remaining,line,'\n');
        std::cout << "*" << line << "*" << std::endl;
        remaining_nl--;
        if (line == "\r")
        {
            std::cout << "blaan hna aasat" << std::endl;
            state = REQUEST_BODY;
            break;
        }
        push_header(line);
    }
    //handle body;
}

int http_request::get_state()
{
    return state;
}

std::string http_request::get_header(const std::string &header)
{
    std::string empty;
    for (int i = 0;i < headers.size();i++)
    {
        if (headers[i].first == header)
            return (headers[i].second);
    }
    return empty;
}

void http_request::parse_remaining(char *buffer,int len, int n_new_line)
{
    void (http_request::*handlers[2])() = {&http_request::http_header_handler,&http_request::headers_handler};



    remaining_nl += n_new_line;
    remaining.write(buffer,len);
    std::cout << "dkhlat" << state << std::endl;
    while (remaining_nl && state != REQUEST_BODY)
        (this->*handlers[state])();
    std::cout << "dkhlat" << state << std::endl;
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
std::stringstream &http_request::get_remaining()
{
    return remaining;
}