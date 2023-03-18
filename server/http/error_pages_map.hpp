#pragma once

#include "../servers.hpp"
#include <utility>


class error_pages_map : public std::map<int, std::string>
{
	private:
		std::string 				def;
		std::map<int, std::string> error_pages;
	public:
		error_pages_map();
		std::string& get_error(int error_code);
};


