#include "../http_response.hpp"

void http_response::DELETE_handler()
{
    if (state == OPEN_STREAM)
        state = RESPONSE_BODY;
}
