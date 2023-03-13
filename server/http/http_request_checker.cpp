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

/*
TO DO:
the mandatory requirements for each HTTP request:

ALL requests:
	The request must contain a valid resource path in the form of an absolute or relative URL.
	The request must contain a valid HTTP version, which must be either HTTP/1.0 or HTTP/1.1.
GET & DELETE requests:
	The method must be GET || DELETE.
POST request:
	The method must be POST.
	The request must include a valid Content-Length header specifying the length of the message body in bytes.
	The request must include a Content-Type header specifying the type of data being sent.
	The message body must contain the data being submitted to the server in the correct format specified by the Content-Type header.
*/


