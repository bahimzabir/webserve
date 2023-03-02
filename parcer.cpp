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

std::vector<std::string>	readFile(std::string file_path) {
	std::fstream				file;
	std::vector<std::string>	cmd;
	std::string					file_content;
	std::string					line;
	std::string					tmp;
	
	file.open(file_path, std::ios::in);
	if (!file.is_open())
	{
		std::cerr << "readFile: can't open file: '" + file_path + "'\n";
		return cmd;
	}
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
		// if (parce_scoop == OUT_SCOOP ) {
		// 	servers.push_back(server());
		// }
	}
	// for (int i = 0;  i < cmd.size(); i++)
	// 	std::cout << cmd[i] << " ";
	return cmd;
}

std::vector<server> getServersInfos(std::string configFilePath) {

	int							parce_scoop = OUT_SCOOP;
	std::string					line;
	int							lines_count = 0;
	std::vector<std::string>	cmd;
	std::vector<server>			servers;
	server						serv;	
	std::string					configs;
	std::string					tmp;
	//int					i = 0;

	cmd = readFile(configFilePath);
	if (cmd.empty())
	{
		servers.push_back(server());
		servers[0].parceMsg = "getServersInfos: configs not found!";
		return servers;
	}
	//std::cout << configs;
	for (int i = 0;  i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	return servers;
}


int main(int arc, char** av) {
	std::vector<server>	servers;
	servers = getServersInfos(av[1]);
	//std::cout << servers[0].parceMsg;
}