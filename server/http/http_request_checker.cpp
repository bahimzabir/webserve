/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_checker.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azabir <azabir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 22:32:46 by azabir            #+#    #+#             */
/*   Updated: 2023/03/13 22:32:48 by azabir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_request.hpp"

void http_request::isValidHttpRequest() {
	bool hasHttpVersion = http_header[2] == "HTTP/1.1";
	bool ResourcePath = http_header[1] != "";
    if (!hasHttpVersion || !ResourcePath)
        throw 400;
	if (http_header[0] == "POST")
	{
        if (get_header("TRANSFER-ENCODING") != "" && get_header("TRANSFER-ENCODING") != "chunked")
            throw 501;
        else if (get_header("TRANSFER-ENCODING") == "chunked")
            return;
        if (get_header("CONTENT-LENGTH") == "")
            throw 400;
        long long amount = strtoll(get_header("CONTENT-LENGTH").c_str(),NULL,10);
        if (amount < 0)
            throw 400;
        if (errno == ERANGE)
            throw 400;
	}
}
