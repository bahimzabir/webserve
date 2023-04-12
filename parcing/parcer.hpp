#ifndef PARCER_HPP
#define PARCER_HPP

#include "libs.hpp"


#define OUT_SCOOP 0
#define SERVER_SCOOP 1
#define ROUTE_SCOOP 2
#define PRE_SERVER_SCOOP 3
#define	DEF_HOST "127.0.0.1"
#define	DEF_PORT "8000"
#define	DEF_MAX_BODY_SIZE INT64_MAX
#define	DEF_ALLOW_METHODS "ALL"
#define	DEF_INDEX "index.html"
#define	DEF_AUTO_INDEX 0
#define DEF_ROOT "/"
#define DEF_COF_PATH "parcing/config.conf"

struct errorPage{
	int			error_num;
	std::string	error_file;
};

struct cgi {
	std::string	cgi_param;
	std::string	cgi_pass;
};

struct route
{
	std::string					route_name;
	std::vector<std::string>	methods;
	std::string					root;
	std::string					return_value;
	std::vector<std::string>	index;
	long						autoindex;
	std::string					upload_pass;
	std::vector<cgi>			cgi_pass;
};

struct config {
	std::vector<std::string>		ports;
	std::string						host;
	std::vector<std::string>		server_names;
	long							client_max_body_size;
	std::vector<route>				routes;
	std::vector<errorPage>			err_pages_struct;
	// to be defined
	std::map<int, std::string>		error_pages;
	bool	operator == (const config& conf);
};
extern std::vector<config> config_info;

class Exception : public std::exception {

	private:	
	 	const std::string	exception_msg;
	public:
		Exception(void) : exception_msg("undefined exception throwed\n") {}
		Exception(const std::string& msg) : exception_msg(msg) {}
		~Exception() _NOEXCEPT {}
		const char* what() const _NOEXCEPT { return exception_msg.c_str(); }
};

std::vector<config> getServersInfos(std::string configFilePath);
void	add_default_params(config& conf, config& deflt);
void	pathTrim(std::string& str);

#endif