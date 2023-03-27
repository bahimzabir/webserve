#include "error_pages_map.hpp"

error_pages_map::error_pages_map(void) {
   def = "<html><body><h1>Error</h1></body></html>";
   error_pages.insert(std::make_pair<int, std::string>(400, "<html><body><h1>Error 400: Bad Request</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(401, "<html><body><h1>Error 401: Unauthorized</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(403, "<html><body><h1>Error 403: Forbidden</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(404, "<html><body><h1>Error 404: Not Found</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(405, "<html><body><h1>Error 405: Method Not Allowed</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(500, "<html><body><h1>Error 500: Internal Server Error</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(501, "<html><body><h1>Error 501: Not Implemented</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(201, "<html><body><h1>File Created</h1></body></html>"));

   msg_def = "Error";
   error_messages.insert(std::make_pair<int, std::string>(400, "Bad Request"));
   error_messages.insert(std::make_pair<int, std::string>(401, "Unauthorized"));
   error_messages.insert(std::make_pair<int, std::string>(403, "Forbidden"));
   error_messages.insert(std::make_pair<int, std::string>(404, "Not Found"));
   error_messages.insert(std::make_pair<int, std::string>(500, "Internal Server Error"));
   error_messages.insert(std::make_pair<int, std::string>(501, "Not Implemented"));
   error_messages.insert(std::make_pair<int, std::string>(405, "Method Not Allowed"));
   error_messages.insert(std::make_pair<int, std::string>(201, "Created"));
}

std::string& error_pages_map::get_error(int error_code) {
   return error_pages[error_code].empty() ? def : error_pages[error_code];
}
std::string& error_pages_map::get_message(int error_code) {
   return error_messages[error_code].empty() ? msg_def : error_messages[error_code];
}