#include "http_request.hpp"

void trim_field(std::string &field)
{
    size_t beg = 0;
    size_t end = 0;
    
    for (size_t i = 0;i < field.size();i++)
    {
        if (isspace(field[i]))
            beg++;
        else
            break;
    }
    for (size_t i = field.size();i > beg; i--)
    {
        if (isspace(field[i - 1]))
            end++;
        else
            break;
    }
    field = field.substr(beg,field.size() - (beg + end));
}


http_request::http_request()
{
    state  = HTTP_HEADER;
    remaining_nl = 0;
}
void http_request::push_header(std::string &line)
{
    std::pair<std::string,std::string> pair;
    std::string field;
    std::stringstream str_stream(line);


    std::getline(str_stream,field,':');
    for (size_t i = 0; i < field.size();i++)
        field[i] = toupper(field[i]);
    pair.first = field;
    
    std::getline(str_stream,field);
    trim_field(field);
    pair.second = field;
    headers.push_back(pair);
}

void http_request::http_header_handler()
{
    if (!remaining_nl)
        return;
    std::string line;
    std::getline(remaining,line,'\n');

    std::stringstream str_stream(line);
    std::string field;

    std::getline(str_stream,field,' ');
    trim_field(field);
    http_header[0] = field;
    std::getline(str_stream,field,' ');
    trim_field(field);
    http_header[1] = field;
    std::getline(str_stream,field,' ');
    trim_field(field);
    http_header[2] = field;
    if ((int)http_header[0][0] == 22)
        throw 666;
    state = REQUEST_HEADERS;
    remaining_nl--;
}

void http_request::headers_handler()
{
    std::string line;
    while (remaining_nl)
    {
        std::getline(remaining,line,'\n');
        remaining_nl--;
        if (line == "\r")
        {
            state = REQUEST_BODY;
            break;
        }
        push_header(line);
    }
}

int http_request::get_state()
{
    return state;
}

std::string http_request::get_header(const std::string &header)
{
    std::string empty;
    for (size_t i = 0;i < headers.size();i++)
    {
        if (headers[i].first == header)
            return (headers[i].second);
    }
    return empty;
}
std::string http_request::get_headers()
{
    std::string head;
    for (size_t i = 0;i < headers.size();i++)
        head += headers[i].first + ": " + headers[i].second + "\n";
    return head;
}

void http_request::parse_remaining(char *buffer,int len, int n_new_line, size_t *out_time)
{
    void (http_request::*handlers[2])() = {&http_request::http_header_handler,&http_request::headers_handler};

    timeout = out_time;
    remaining_nl += n_new_line;
    remaining.write(buffer,len);
    while (remaining_nl && state != REQUEST_BODY)
        (this->*handlers[state])();
}


http_request &http_request::operator=(const http_request &obj)
{
    remaining_nl = 0;
    state  = HTTP_HEADER;
    state = obj.state;
    return *this;
}

http_request::http_request(const http_request &obj)
{
    remaining_nl = 0;
    state  = HTTP_HEADER;
    state = obj.state;
    return;
}

void http_request::reset()
{
    state = REQUEST_HEADERS;
    remaining_nl = 0;
    remaining.clear();
    headers.clear();
}
const std::string &http_request::get_method()
{
    return http_header[0];
}
std::string &http_request::get_path()
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