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

bool http_request::isValidHttpRequest() {
	
	bool hasResourcePath = false;
    bool hasHttpVersion = false;
    bool hasContentLength = false;
    bool hasContentType = false;
    bool hasMessageBody = false;


	hasResourcePath = http_header[2] == "HTTP/1.1" ? true : false;
	hasHttpVersion = !http_header[1].empty() ? true : false;
	if ((http_header[0] == "GET" || http_header[0] == "DELETE"))
			return hasResourcePath && hasHttpVersion;
	else if (http_header[0] == "POST")
	{
		for(int i = 0; i < headers.size(); i++) {
			if (headers[i].first == "CONTENT-TYPE")
				hasContentType = true;
			else if (headers[i].first == "TRANSFER-ENCODING" || headers[i].first == "CONTENT-LENGTH")
				hasContentLength = true;
			else if (headers[i].first == "MESSAGE-BODY")
				hasMessageBody = true;
		}
		return hasResourcePath && hasHttpVersion && hasContentLength && hasContentType && hasMessageBody;
	}
	else
		return false;
}
