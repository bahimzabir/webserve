#include "servers.hpp"
#include "http/error_pages_map.hpp"

void	init_default_params(config& conf) {
	error_pages_map def_error_p;

	if (conf.ports.empty())
		conf.ports.push_back(DEF_PORT);
	conf.host.empty() ? DEF_HOST : conf.host;
	conf.client_max_body_size = !conf.client_max_body_size ? DEF_MAX_BODY_SIZE : conf.client_max_body_size;
	if (conf.routes.empty()) 
		conf.routes.push_back(route());
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
	
	for (std::vector<config>::iterator it = configs.begin(); it != configs.end(); it++) {
		config& cf = *it;

	}
}