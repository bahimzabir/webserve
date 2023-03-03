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
#define ROUTE_SCOOP 2
#define PRE_SERVER_SCOOP 3
#define PRE_ROUTE_SCOOP 4
#define PRE_ROUTE_NAME 5

struct errorPage {
	std::string	error_num;
	std::string	error_file;
};

struct cgi {
	std::string	cgi_param;
	std::string	cgi_pass;
};

struct route
{
	std::string					route_name;
	std::vector<std::string>	methods;
	std::string					root;
	std::string					return_value;
	std::vector<std::string>	index;

	// TO BE DEFINED:

	std::string					autoindex;
	std::string					upload_pass;
	std::vector<cgi>			cgi_pass;
};

struct server {
	std::vector<std::string>	ports;
	std::string					host;
	std::vector<std::string>	server_names;
	std::string					client_max_body_size;
	std::vector<route>			routes;
	std::vector<errorPage>		error_pages;

	//config file parcing status variables:

	bool						seccessParce;
	std::string					parceMsg;
};

class Exception : public std::exception {

	private:	
	 	const std::string	exception_msg;
	public:
		Exception(void) : exception_msg("undefined exception throwed\n") {}
		Exception(const std::string& msg) : exception_msg(msg) {}
		~Exception() _NOEXCEPT {}
		const char* what() const _NOEXCEPT { return exception_msg.c_str(); }
};

std::vector<server> getServersInfos(std::string configFilePath);

#endif