#include "servers.hpp"
#include "http/error_pages_map.hpp"

void	init_default_params(config& conf) {
	if (conf.ports.empty())
		conf.ports.push_back(DEF_PORT);
	conf.host.empty() ? DEF_HOST : conf.host;
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
			rt.root = rt.root.empty() ? DEF_ROOT : rt.root;
			if(rt.index.empty())
				rt.index.push_back(DEF_INDEX);
				//here we can fix root name
			rt.route_name = rt.route_name.empty() ? DEF_ROOT : rt.route_name;
	}
}

void server_init(std::vector<config>& configs) {
	
	configs = getServersInfos("parcing/config.conf");
	for (std::vector<config>::iterator it = configs.begin(); it != configs.end(); it++) {
		config& cf = *it;
		init_default_params(cf);
	}
}