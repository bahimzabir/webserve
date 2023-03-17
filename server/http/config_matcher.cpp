#include "config_matcher.hpp"

config_match& config_struct_fill(config& cf, route& rout, std::string& cmbs, std::vector<errorPage>& error_p) {

	config_match *conf = new config_match();

	conf->methods = rout.methods;
	conf->client_max_body_size = cmbs;
	conf->root = rout.root;
	conf->index = rout.index;
	conf->autoindex = rout.autoindex;
	conf->upload_pass = rout.upload_pass;
	conf->cgi_pass = rout.cgi_pass;
	conf->error_pages = error_p;
	conf->return_value = rout.return_value;
	conf->cgi_pass = rout.cgi_pass;
	return *conf;
}

config_match& get_config(std::string host, std::string port, std::string rout, std::string server_name, std::vector<config>& config_info, config& def) {

	config* match_lvl1 = NULL;
	config* match_lvl2 = NULL;
	config* match_lvl3 = NULL;
	std::vector<route>::iterator r_it;
	route rt;
	for (std::vector<config>::reverse_iterator it = config_info.rbegin(); it != config_info.rend(); it++) {
		config& cf = *it;
		if (cf.host == host && std::find(cf.ports.begin(), cf.ports.end(), port) != cf.ports.end())
		{	
			match_lvl1 = &cf;
			if (std::find(cf.server_names.begin(), cf.server_names.end(), server_name) != cf.server_names.end())
			{
				match_lvl2 = &cf;
				r_it = cf.routes.begin();
				while(r_it != cf.routes.end()) {
					rt = *r_it;
					if (rt.route_name == rout)
						break;
					r_it++;
				}
				if (r_it != cf.routes.end())
					match_lvl3 = &cf;
			}
		}
	}

	//std::cerr << "debug = "<< match_lvl3->host << "\n";
	

	if (match_lvl3)
	{
		std::cerr << "here\n";
		// std::cerr << match_lvl3.size() << "\n";
		return (config_struct_fill(*match_lvl3, rt, match_lvl3->client_max_body_size, match_lvl3->error_pages));
	}else if(match_lvl2)
	{
		return (config_struct_fill(*match_lvl2, *def.routes.begin(), match_lvl2->client_max_body_size, match_lvl2->error_pages));
	}else if (match_lvl1)
	{
		return (config_struct_fill(*match_lvl1, *def.routes.begin(), match_lvl1->client_max_body_size, match_lvl1->error_pages));
	}else
	{
		return (config_struct_fill(def, *def.routes.begin(), def.client_max_body_size, def.error_pages));
	}
	// std::cerr << "here\n";
}
