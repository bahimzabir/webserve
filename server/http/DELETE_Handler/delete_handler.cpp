#include "../http_response.hpp"

void http_response::DELETE_handler()
{
    if (state == FILE)
        state = RESPONSE_BODY;
}
