#pragma once

#include <map>
#include <utility>
#include <string>

class error_pages_map : public std::map<int, std::string>
{
	private:
		std::map<int, std::string> error_pages;
        std::map<int, std::string> error_messages;
	public:
		error_pages_map();
		std::string& get_error(int error_code);
        std::string& get_message(int error_code);
};


