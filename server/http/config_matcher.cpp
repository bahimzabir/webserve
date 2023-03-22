#include "config_matcher.hpp"
#include <algorithm>

config_match& config_struct_fill(route& rout, long cmbs, std::map<int, std::string>& error_p, std::string newRoot) {

	config_match *conf = new config_match();

	conf->methods = rout.methods;
	conf->client_max_body_size = cmbs;
	conf->root = newRoot;
	// if (rout.root.back() != '/')
	// 	conf->root += "/";
	pathTrim(conf->root);
	conf->root = "/" + conf->root;
	std::cerr << conf->root << "\n";
	conf->index = rout.index;
	conf->autoindex = rout.autoindex;
	conf->upload_pass = rout.upload_pass;
	conf->cgi_pass = rout.cgi_pass;
	conf->err_pages = error_p;
	conf->return_value = rout.return_value;
	conf->cgi_pass = rout.cgi_pass;
	return *conf;
}

route& rout_matching(std::vector<route> routes, std::string rt_name) 
{	
	route *rt = NULL;
	int rt_len = rt_name.size();
	int	vc_len = routes.size();
	int r = 0;

	//std::sort(routes , routes.end());
	for (int i = 0; i < vc_len; i++) {
		if (routes[i].route_name.size() <= rt_len)
		{
			//std::cerr << "rout needed = " + rt_name + " to match = "+ routes[i].route_name + " size " << routes[i].route_name.size() << "\n";
			if(rt_name.compare(0, routes[i].route_name.size(), routes[i].route_name) == 0) {
				rt = &routes[i];
			}
		}
	}
	if (rt)
		return *rt;
	else
		return (config_info.back().routes[0]);
}

/*

TODO:
	nginx chooses the shortest matched path
	when  requesting /dir/dir1, and the cofing has a rout of name of /dir and root /server/path
		i have to return /server/path/dir1
	to recieve: dir/mana/lolo/wowo => len is important
				dir/mana/lolo
				dir/
				dir/mana
				/
*/

config_match& get_config(std::string host, std::string port, std::string rout, std::string server_name) {

	config* match_lvl1 = NULL;
	config* match_lvl2 = NULL;
	config* match_lvl3 = NULL;
	config* match_lvl4 = &config_info.back();
	std::vector<route>::iterator r_it;
	route rt;
	route rt2;
	std::string rout2;
	rout2 = rout;

	pathTrim(server_name);
	pathTrim(rout);
	std::cerr << "[" + server_name + "]\n";
	for (std::vector<config>::reverse_iterator it = config_info.rbegin(); it != config_info.rend(); it++) {
		config& cf = *it;
		if (cf.host == host && std::find(cf.ports.begin(), cf.ports.end(), port) != cf.ports.end())
		{	
			match_lvl1 = &cf;
			if (std::find(cf.server_names.begin(), cf.server_names.end(), server_name) != cf.server_names.end())
			{
				rt2 = rout_matching(cf.routes, rout);
				// std::cerr << "matching result of " + rout + " = " + rt2.route_name + "\n";
				rout2.replace(0, rt2.route_name.size() + 1 ,rt2.root);
				std::cerr << "result  = " + rout2 + "\n";

				match_lvl2 = &cf;
				// r_it = cf.routes.begin();
				// while(r_it != cf.routes.end()) {
				// 	rt = *r_it;
				// 	if (rt.route_name == rout)
				// 		break;
				// 	r_it++;
				// }
				// if (r_it != cf.routes.end())
				// 	match_lvl3 = &cf;
			}
		}
	}

	// if (match_lvl3)
	// 	return (config_struct_fill(rt, match_lvl3->client_max_body_size, match_lvl3->error_pages));
	if(match_lvl2)
		return (config_struct_fill(rt2, match_lvl2->client_max_body_size, match_lvl2->error_pages, rout2));
	else if(match_lvl1)
		return (config_struct_fill(*match_lvl4->routes.begin(), match_lvl1->client_max_body_size, match_lvl1->error_pages, rout2));
	else
		return (config_struct_fill(*match_lvl4->routes.begin(), match_lvl4->client_max_body_size, match_lvl4->error_pages, rout2));
}
