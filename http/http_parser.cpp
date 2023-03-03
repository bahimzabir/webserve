#include "http_parser.hpp"


void check_error()
{
    ;
}


void make_pairs(std::vector<std::string> &lines, std::vector<std::pair <std::string,std::string>> &headers)
{
    int end;
    std::pair<std::string,std::string> ;
    for (int i = 0;i < lines.size();i++)
    {
        end = lines[i].find(":",0);
        if (end == std::string::npos)
        {
            std::cout << "bad_request" << std::endl;
        }
        lines[i].
    }
}

void parse_headers(std::string &buffer,std::vector<std::pair <std::string,std::string>> &headers)
{
    int start = 0;
    int end = 0;
    std::string substring;
    std::vector<std::string> lines;
    while (1)
    {
        end = buffer.find('\n',start);
        if (end == std::string::npos)
        {
            lines.push_back(buffer.substr(start,lines.size()));
            break;
        }
        lines.push_back(buffer.substr(start,end));
        start = end;
    }
    make_pairs(,);
}




void http_parser(std::string &buffer)
{
    std::vector<std::pair <std::string,std::string>> headers;
    
}