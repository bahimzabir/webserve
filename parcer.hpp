/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azabir <azabir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:26:19 by azabir            #+#    #+#             */
/*   Updated: 2023/03/01 16:26:20 by azabir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARCER_HPP
#define PARCER_HPP

#include "libs.hpp"

#define OUT_SCOOP 0
#define SERVER_SCOOP 1
#define ROOT_SCOOP 2

struct errorPage {
	int			error_num;
	std::string	error_file;
};

struct route
{
	std::vector<std::string>	route_name;
	std::vector<std::string>	methods;
	std::vector<std::string>	root;
	std::vector<std::string>	index;

	// TO BE DEFINED:

	std::vector<std::string>	upload_store;
	std::vector<std::string>	upload_pass;
	std::vector<std::string>	upload_pass_args;
	std::vector<std::string>	fastcgi_pass;
	std::vector<std::string>	fastcgi_param;
};

struct server {
	std::vector<std::string>	ports;
	std::vector<std::string>	server_names;
	std::vector<std::string>	client_max_body_size;
	std::vector<route>			routes;
	std::vector<errorPage>		error_pages;

	//config file parcing status variables:

	bool						seccessParce;
	std::string					parceMsg;
};

std::vector<server> getServersInfos(std::string configFilePath);

#endif