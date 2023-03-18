/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azabir <azabir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:25:41 by azabir            #+#    #+#             */
/*   Updated: 2023/03/01 16:25:42 by azabir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parcer.hpp"

bool	is_special(char c) {
	if (c == '{' || c == ';' || c == '}' || c == ' ' || c == '\n')
		return true;
	return false;
}
bool is_digit(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

std::string line_num(std::vector<std::string> cmd, int i) {

	int line_num = 1;
	int	j = 0;

	while (j < i) {
		if (cmd[j] == "\n")
			line_num++;
		j++;
	}

	return std::to_string(line_num);
}

void scoopCheck(std::vector<std::string> cmd) {
	int i = 0;


}

void	add_default_params(config& conf, config& deflt) {
	
	conf.ports = conf.ports.empty() ? deflt.ports : conf.ports;
	conf.host = conf.host.empty() ? deflt.host : conf.host;
	conf.server_names = conf.server_names.empty() ? deflt.server_names : conf.server_names;
	conf.client_max_body_size = conf.client_max_body_size.empty() ? deflt.client_max_body_size : conf.client_max_body_size;
	conf.error_pages = conf.error_pages.empty() ? deflt.error_pages : conf.error_pages;
	if (conf.routes.empty()) 
		conf.routes = deflt.routes;
	else {
		for (std::vector<route>::iterator it = conf.routes.begin(); it != conf.routes.end(); it++) {
			route &rt = *it;
			rt.autoindex = rt.autoindex.empty() ? deflt.routes[0].autoindex : rt.autoindex;
			rt.methods = rt.methods.empty() ? deflt.routes[0].methods : rt.methods;
			rt.root = rt.root.empty() ? deflt.routes[0].root : rt.root;
			rt.return_value = rt.return_value.empty() ? deflt.routes[0].return_value : rt.return_value;
			rt.index = rt.index.empty() ? deflt.routes[0].index : rt.index;
			rt.route_name = rt.route_name.empty() ? deflt.routes[0].route_name : rt.route_name;
			rt.upload_pass = rt.upload_pass.empty() ? deflt.routes[0].upload_pass : rt.upload_pass;
			rt.cgi_pass = rt.cgi_pass.empty() ? deflt.routes[0].cgi_pass : rt.cgi_pass;
		}
	}
}

std::vector<std::string>	readFile(std::string file_path) {

	std::fstream				file;
	std::vector<std::string>	cmd;
	std::string					file_content;
	std::string					line;
	std::string					tmp;
	
	file.open(file_path, std::ios::in);
	if (!file.is_open())
		throw Exception("readFile: can't open file: '" + file_path + "'\n");
	while (std::getline(file, line))
		file_content += line + "\n";
	file.close();
	for(int i = 0; i < file_content.length(); i++) {
		
		while (!is_special(file_content[i])) {
			tmp += file_content[i];
			i++;
		}
		if (tmp != "")
			cmd.push_back(tmp);
		if (file_content[i] != ' ' && is_special(file_content[i])) {
			tmp = file_content[i];
			cmd.push_back(tmp);
		}
		tmp = "";
	}
	return cmd;
}

int	getStringValue(std::string& configFilePath, std::string& toReplace, std::vector<std::string>& cmd, std::string toGet, int servers_index, int i) {
	i++;
	if (cmd[i] == ";" || cmd[i] == "\n")
		throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": no Host defined for the server\n");
	if (toGet == "autoindex" && cmd[i] != "0" && cmd[i] != "1")
		throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": 'autoindex' has to be 0 or 1\n");
	if ((toGet == "max_client_body_size" || toGet == "autoindex") && !is_digit(cmd[i]))
		throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" + toGet + "' has to be a number\n");
	toReplace = cmd[i++];
	if (cmd[i] == "\n")
		throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
	if (cmd[i] != ";")
		throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": too many arguments for '" + toGet + "'\n");
	i++;
	return i;
}

std::vector<config> getServersInfos(std::string configFilePath) {

	int							parce_scoop = OUT_SCOOP;
	int							servers_index = -1;
	int							error_pages_index = -1;
	int							routes_index = -1;
	int							cgi_pass_index = -1;
	std::vector<std::string>	cmd;
	std::vector<config>			servers;

	cmd = readFile(configFilePath);
	for (int i = 0;  i < cmd.size(); i++) {
		error_pages_index = -1;
		routes_index = -1;
		while(i < cmd.size() && cmd[i] == "\n")
			i++;
		if (i >= cmd.size())
			break;
		if (parce_scoop == OUT_SCOOP) {
			if (cmd[i] != "server") {
				throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" +cmd[i] +"' is undefined, Do you mean 'server'\n");
			}
			parce_scoop = PRE_SERVER_SCOOP;
		}
		else if (parce_scoop == PRE_SERVER_SCOOP) {
			if (cmd[i] != "{") {
				throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" +cmd[i] +"' is undefined, '{' is expected after 'server' definition\n");
			}
			parce_scoop = SERVER_SCOOP;
			servers_index++;
			servers.push_back(config());
		}
		else if (parce_scoop == SERVER_SCOOP) {
			while (i < cmd.size()) {
				while(cmd[i] == "\n")
					i++;
				if (cmd[i] == "listen") {
					i++;
					if (cmd[i] == ";" || cmd[i] == "\n")
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": no ports defined for the server\n");
					while (cmd[i] != ";") {
						servers[servers_index].ports.push_back(cmd[i]);
						if (cmd[i] == "\n")
							throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
						i++;
					}
				} else if (cmd[i] == "server_name") {
					i++;
					if (cmd[i] == ";" || cmd[i] == "\n")
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": no names defined for the server\n");
					while (cmd[i] != ";") {
						servers[servers_index].server_names.push_back(cmd[i]);
						if (cmd[i] == "\n")
							throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
						i++;
					}
				} else if (cmd[i] == "host") {
					i = getStringValue(configFilePath, servers[servers_index].host, cmd, "host", servers_index, i);
				} else if (cmd[i] == "max_client_body_size") {
					i = getStringValue(configFilePath, servers[servers_index].client_max_body_size, cmd, "max_client_body_size", servers_index, i);
				} else if (cmd[i] == "error_page") {
					i++;
					servers[servers_index].error_pages.push_back(errorPage());
					error_pages_index++;
					if (cmd[i] == ";" || cmd[i] == "\n" || !is_digit(cmd[i]))
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": "+ cmd[i] +" 'error code' is invalid for the error_page\n");
					servers[servers_index].error_pages[error_pages_index].error_num = cmd[i++];
					if (cmd[i] == ";" || cmd[i] == "\n" )
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": 'error page' is required\n");
					servers[servers_index].error_pages[error_pages_index].error_file = cmd[i++];
					if (cmd[i] == "\n")
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
					else if (cmd[i] != ";")
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": too many arguments for error_page\n");
					i++;
	
				} else if (cmd[i] == "location") {
					cgi_pass_index = -1;
					i++;
					if (i >= cmd.size() || is_special(cmd[i][0]))
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": location path is required\n");
					servers[servers_index].routes.push_back(route());
					routes_index++;
					servers[servers_index].routes[routes_index].route_name = cmd[i];
					i++;
					while(cmd[i] == "\n")
						i++;
					if (cmd[i] != "{")
						throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" +cmd[i] +"' is undefined, '{' is expected after 'location path' definition\n");
					i++;
					while(cmd[i] == "\n")
						i++;
					parce_scoop = ROUTE_SCOOP;
					while (parce_scoop == ROUTE_SCOOP) {
						while(cmd[i] == "\n")
							i++;
						if (cmd[i] == "allow_methods") {
							i++;
							if (cmd[i] == ";" || cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": no 'allow_methods' defined for the route\n");
							while (cmd[i] != ";") {
								servers[servers_index].routes[routes_index].methods.push_back(cmd[i]);
							if (cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
							i++;
							}
						}else if (cmd[i] == "index") {
							i++;
							if (cmd[i] == ";" || cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": no 'index' defined for the route\n");
							while (cmd[i] != ";") {
								servers[servers_index].routes[routes_index].index.push_back(cmd[i]);
							if (cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
							i++;
							}
						}else if (cmd[i] == "autoindex") {
							i = getStringValue(configFilePath, servers[servers_index].routes[routes_index].autoindex, cmd, "autoindex", servers_index, i);
						}else if (cmd[i] == "root") {
							i = getStringValue(configFilePath, servers[servers_index].routes[routes_index].root, cmd, "root", servers_index, i);
						}else if (cmd[i] == "upload_pass") {
							i = getStringValue(configFilePath, servers[servers_index].routes[routes_index].upload_pass, cmd, "upload_pass", servers_index, i);
						} else if (cmd[i] == "return") {
							i = getStringValue(configFilePath, servers[servers_index].routes[routes_index].return_value, cmd, "return", servers_index, i);
						} else if (cmd[i] == "cgi_pass") {
							i++;
							servers[servers_index].routes[routes_index].cgi_pass.push_back(cgi());
							cgi_pass_index++;
							if (cmd[i] == ";" || cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": "+ cmd[i] +" 'cgi_param' is invalid for the cgi\n");
							servers[servers_index].routes[routes_index].cgi_pass[cgi_pass_index].cgi_pass = cmd[i];
							i++;
							if (cmd[i] == ";" || cmd[i] == "\n" )
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": 'cgi_pass' is required\n");
							servers[servers_index].routes[routes_index].cgi_pass[cgi_pass_index].cgi_param = cmd[i++];
							if (cmd[i] == "\n")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": ';' is expected\n");
							else if (cmd[i] != ";")
								throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": too many arguments for cgi\n");
							i++;
						} else if (cmd[i] == "}")
							parce_scoop = SERVER_SCOOP;
						else
							throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" + cmd[i] + "' is not defined in location\n");
						i++;
					}
				} else if (cmd[i] == "}") {
					parce_scoop = OUT_SCOOP;
					//i++;
					break;
				}
				else
					throw Exception(configFilePath + ":line " + line_num(cmd, i) + ": '" + cmd[i] + "' is not defined in server\n");
				i++;
			}
		}
	}
	return servers;
}


// int main(int arc, char** av) {
// 	std::vector<config>	servers;
// 	try {
// 		servers = getServersInfos(av[1]);
// 		std::cout << "Number of servers: " << servers.size() <<"\n";
// 	} catch (std::exception& obj) {
// 		std::cout << obj.what();
// 	}
// }