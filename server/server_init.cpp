#include "servers.hpp"
#include "http/error_pages_map.hpp"

std::vector<config> config_info;

void	strTrim(std::string& str) {
    int i = 0;
    int j = str.size() - 1;
    if (i <= j && std::isspace(str[i])) {
        i++;
    }
    if (j > 0 && (std::isspace(str[j]) || str[j] == '/')) {
       j--;
    }
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
			if(rt.index.empty())
				rt.index.push_back(DEF_INDEX);
			rt.root = rt.root.empty() ? DEF_ROOT : rt.root;
			if (rt.route_name != "/")
				strTrim(rt.route_name);
			if (rt.root != "/")
				strTrim(rt.root);
	}
}

void server_init() {
	config_info = getServersInfos("parcing/config.conf");
	config_info.push_back(config());
	//std::cerr << "debug = " << configs.back().routes.size() << "\n";
	for (std::vector<config>::iterator it = config_info.begin(); it != config_info.end(); it++) {
		config& cf = *it;
		init_default_params(cf);
	}
}