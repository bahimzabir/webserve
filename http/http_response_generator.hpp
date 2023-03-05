#include "http_request.hpp"

class http_response
{
    http_request &request;
    std::string headers;
    std::string body;
    public:
        http_response(http_request &req);
};