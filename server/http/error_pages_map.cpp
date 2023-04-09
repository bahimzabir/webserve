#include "error_pages_map.hpp"

error_pages_map::error_pages_map(void) {
   error_pages.insert(std::make_pair<int, std::string>(400, "<html><body><h1>Error 400: Bad Request</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(401, "<html><body><h1>Error 401: Unauthorized</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(403, "<html><body><h1>Error 403: Forbidden</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(404, "<html><body><h1>Error 404: Not Found</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(405, "<html><body><h1>Error 405: Method Not Allowed</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(500, "<html><body><h1>Error 500: Internal Server Error</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(501, "<html><body><h1>Error 501: Not Implemented</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(201, "<html><body><h1>File Created</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(204, ""));
   error_pages.insert(std::make_pair<int, std::string>(301, ""));
   error_pages.insert(std::make_pair<int, std::string>(413, "<html><body><h1>Request Entity Too Large</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(409, "<html><body><h1>Conflict</h1></body></html>"));

   error_messages.insert(std::make_pair<int, std::string>(400, "Bad Request"));
   error_messages.insert(std::make_pair<int, std::string>(401, "Unauthorized"));
   error_messages.insert(std::make_pair<int, std::string>(403, "Forbidden"));
   error_messages.insert(std::make_pair<int, std::string>(404, "Not Found"));
   error_messages.insert(std::make_pair<int, std::string>(500, "Internal Server Error"));
   error_messages.insert(std::make_pair<int, std::string>(501, "Not Implemented"));
   error_messages.insert(std::make_pair<int, std::string>(405, "Method Not Allowed"));
   error_messages.insert(std::make_pair<int, std::string>(201, "Created"));
   error_messages.insert(std::make_pair<int, std::string>(204, "No Content"));
   error_messages.insert(std::make_pair<int, std::string>(301, "Moved Permanently"));
   error_messages.insert(std::make_pair<int, std::string>(413, "Request Entity Too Large"));
   error_messages.insert(std::make_pair<int, std::string>(409, "Conflict"));
}

std::string& error_pages_map::get_error(int error_code) {
   return error_pages[error_code];
}
std::string& error_pages_map::get_message(int error_code) {
   return error_messages[error_code];
}