#include "http_request.hpp"

void make_pairs(http_request &request,std::string &line)
{
    std::stringstream str_stream(line);
    std::pair<std::string,std::string> pair;
    std::string field;

    getline(str_stream,field,':');
    pair.first = field;
    getline(str_stream,field,':');
    pair.second = field;
    request.headers.push_back(pair);
}

void get_method(http_request &request,std::string &line)
{
    std::stringstream str_stream(line);
    std::string field;

    getline(str_stream,field,' ');
    request.http_header[0] = field;
    getline(str_stream,field,' ');
    request.http_header[1] = field;
    getline(str_stream,field,' ');
    request.http_header[2] = field;
}

void parse_headers(http_request &request)
{
    std::stringstream str_stream(request.req);
    std::string line;

    getline(str_stream,line,'\n');
    get_method(request,line);
    while (1)
    {
        getline(str_stream,line,'\n');
        if (line == "")
            break;
        make_pairs(request,line);
        if (str_stream.eof())
            break;
    }
    while(!str_stream.eof())
    {
        getline(str_stream,line);
        request.body += line;
    }
    //handle body;
}


void http_parser(http_request &request)
{
    parse_headers(request);
}