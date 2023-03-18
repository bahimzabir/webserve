#include "error_pages_map.hpp"

error_pages_map::error_pages_map(void) {
   def = "<html><body><h1>Error</h1></body></html>";
   error_pages.insert(std::make_pair<int, std::string>(400, "<html><body><h1>Error 400: Bad Request</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(401, "<html><body><h1>Error 401: Unauthorized</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(403, "<html><body><h1>Error 403: Forbidden</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(404, "<html><body><h1>Error 404: Not Found</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(500, "<html><body><h1>Error 500: Internal Server Error</h1></body></html>"));
   error_pages.insert(std::make_pair<int, std::string>(503, "<html><body><h1>Error 503: Service Unavailable</h1></body></html>"));
}

std::string& error_pages_map::get_error(int error_code) {
   return error_pages[error_code].empty() ? def : error_pages[error_code];
}