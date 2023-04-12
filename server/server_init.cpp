#include "servers.hpp"
#include "http/error_pages_map.hpp"

std::vector<config> config_info;

void	check_ports() {
	int size = config_info.size();
	std::map<std::string, std::vector<std::string> > np;
	for (int i = 0; i < (size - 1); i++) {
		for (int j = i + 1; j < size - 1; j++) {
			if (config_info[i] == config_info[j])
				throw Exception("fatal: servers ports conflict, cannot init the server\n"); 
		}
	}
}

void	pathTrim(std::string& str) {
    int i = 0;
    int j = str.size() - 1;
    while (i <= j && (std::isspace(str[i]) || str[i] == '/')) {
        i++;
    }
    while (j > 0 && (std::isspace(str[j]) || str[j] == '/')) {
       j--;
    }
	str = "/" + str;
    str = str.substr(i, j + 1);
}

void	init_default_params(config& conf) {
	if (conf.ports.empty())
		conf.ports.push_back(DEF_PORT);
	conf.host = conf.host.empty() ? DEF_HOST : conf.host;
	conf.client_max_body_size = !conf.client_max_body_size ? DEF_MAX_BODY_SIZE : conf.client_max_body_size;
	if (conf.routes.empty()) 
		conf.routes.push_back(route());
	if (!conf.err_pages_struct.empty())
	{
		for (std::vector<errorPage>::iterator it = conf.err_pages_struct.begin(); it != conf.err_pages_struct.end(); it++) {
			errorPage& err = *it;
			conf.error_pages.insert(std::make_pair<int, std::string>(err.error_num, err.error_file));
		}
	}
	for (std::vector<route>::iterator it = conf.routes.begin(); it != conf.routes.end(); it++) {
			route &rt = *it;
			if (rt.methods.empty())
				rt.methods.push_back(DEF_ALLOW_METHODS);
			// if(rt.index.empty())
			// 	rt.index.push_back(DEF_INDEX);
			rt.root = rt.root.empty() ? std::getenv("PWD") : rt.root;
			if (rt.route_name != "/")
				pathTrim(rt.route_name);
			if (rt.root != "/")
				pathTrim(rt.root);
	}
}

void server_init(std::string confPath) {
	config_info = getServersInfos(confPath);
	if (config_info.size() == 0)
		config_info.push_back(config());
	config_info.push_back(config());
	for (std::vector<config>::iterator it = config_info.begin(); it != config_info.end(); it++) {
		config& cf = *it;
		init_default_params(cf);
	}
	check_ports();
}