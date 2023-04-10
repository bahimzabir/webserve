#include "config_matcher.hpp"
#include <algorithm>

std::vector<std::string> pathToVec(std::string& path) {

	std::vector<std::string>	vpath;
	size_t						len = path.length();
	std::string 				tmp;

	//vpath.push_back("/");
	for(size_t i = 0; i < len; i++) {
	
		while (i < len && path[i] != '/') {
			tmp += path[i];
			i++;
		}
		if (tmp != "") {
			if (tmp == "..")
			{
				std::cerr << "throw\n";
				throw 403;

			}
			vpath.push_back(tmp);
		}
		tmp = "";
	}
	return vpath;
}

config_match& config_struct_fill(route& rout, long cmbs, std::map<int, std::string>& error_p, std::string newRoot, config_match *conf) {

	conf->methods = rout.methods;
	conf->client_max_body_size = cmbs;
	conf->root = newRoot;
	// if (rout.root.back() != '/')
	// 	conf->root += "/";
	pathTrim(conf->root);
	// std::cerr <<"f_root = "<< conf->root << "\n";
	conf->index = rout.index;
	conf->autoindex = rout.autoindex;
	conf->upload_pass = rout.upload_pass;
	conf->cgi_pass = rout.cgi_pass;
	conf->err_pages = error_p;
	conf->return_value = rout.return_value;
	conf->cgi_pass = rout.cgi_pass;
	return *conf;
}

route& rout_matching(std::vector<route> routes, std::string rt_name, std::string& rout2) 
{	
	route *rt = NULL;

	if (rt_name == "")
		rt_name = "//";
	int rt_len;
	int	vc_len = routes.size();
	std::vector<std::string> name_vpath;
	std::vector<std::string> match_vpath;
	std::vector<std::string> root_vpath;
	std::string tmp;

	name_vpath = pathToVec(rt_name);
	rt_len = name_vpath.size();
	for (int i = 0; i < vc_len; i++) {
		match_vpath = pathToVec(routes[i].route_name);
		if (match_vpath.size() <= rt_len)
		{
			tmp = "";
			// std::cerr << "rout_needed= ";
			// for(int i = 0; i < rt_len; i++)
			// 	std::cerr << "/" + name_vpath[i];
			// std::cerr << " ";
			// std::cerr << "to_much= ";
			// for(int i = 0; i < match_vpath.size(); i++)
			// 	std::cerr << "/" + match_vpath[i];
			// std::cerr << "\n";
			if(std::equal(match_vpath.begin(), match_vpath.end(), name_vpath.begin())) {
				rt = &routes[i];
				root_vpath = pathToVec(routes[i].root);
				for(int i = 0; i < root_vpath.size(); i++)
					tmp = tmp + "/" + root_vpath[i];
				for(int i = match_vpath.size(); i < rt_len; i++)
					tmp = tmp + "/" + name_vpath[i];
				rout2 = tmp;
				// std::cerr << "\nnew_root = " << rout2 << "\n\n";

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

config_match& get_config(std::string host, std::string port, std::string rout, std::string server_name, config_match *conf) {

	config* match_lvl1 = NULL;
	config* match_lvl2 = NULL;
	config* match_lvl3 = &config_info.back();
	std::vector<route>::iterator r_it;
	route rt;
	std::string rout2;
	rout2 = rout;

	pathTrim(server_name);
	pathTrim(rout);
	// std::cerr << "[" + server_name + "]\n";
	// std::cerr << "[" + rout + "]\n";
	// std::cerr << "[" + host + "]\n";
	// std::cerr << "[" + port + "]\n";
	for (std::vector<config>::reverse_iterator it = config_info.rbegin(); it != config_info.rend(); it++) {
		config& cf = *it;
		if (cf.host == host && std::find(cf.ports.begin(), cf.ports.end(), port) != cf.ports.end())
		{	
			match_lvl1 = &cf;
			rt = rout_matching(cf.routes, rout, rout2);
			// std::cerr << "rout2 = " << rout2 << "\n";
			if (std::find(cf.server_names.begin(), cf.server_names.end(), server_name) != cf.server_names.end())
				match_lvl2 = &cf;
		}
	}

	if(match_lvl2)
		return (config_struct_fill(rt, match_lvl2->client_max_body_size, match_lvl2->error_pages, rout2, conf));
	else if(match_lvl1) {
		return (config_struct_fill(rt, match_lvl1->client_max_body_size, match_lvl1->error_pages, rout2, conf));
	}
	else
		return (config_struct_fill(*match_lvl3->routes.begin(), match_lvl3->client_max_body_size, match_lvl3->error_pages, rout2, conf));
}
