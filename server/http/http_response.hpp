#pragma once
#include "http_request.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include "../../parcing/parcer.hpp"
#include "config_matcher.hpp"
#include <dirent.h>
#include "error_pages_map.hpp"
#include <poll.h>
#include <cstdio>
#include <sys/time.h>
#include <signal.h>


#define FILE 1
#define LIST_DIRECTORY 2
#define RESPONSE_BODY 3
#define CHUNKED 4
#define NORMAL 5
#define UPLOADED 6
#define RESPONSE_ERROR 8
#define EXECUTOR 9
#define WAITER 10
#define PARSER 11


#define GET 0 
#define POST 1
#define DELETE 2
#define CGI 2
#define SEND 3


#define NOT_IMPLEMENTED 501
#define NOT_ALLOWED 405
#define END 666
#define CREATED 201
#define BAD_REQUEST 400
#define SERVER_ERROR 500
#define FORBIDDEN 403
#define NOT_FOUND 404
#define NO_CONTENT 204
#define BAD_GATEWAY 502
#define ENTITY_LARGE 413
#define _CONFLICT 409
#define GATEWAY_TIMEOUT 504

struct t_cgi_data
{
	int	pid;
	std::string	input;
	int	input_fd;
	std::string	output;
	int	output_fd;
};

class http_response
{
    http_request *request;


    std::string status[3];
    std::map<std::string,std::string> headers;
    std::map<std::string,std::string> content_type;
    std::string res_header;
    long long content_remaining;
    std::string body;
    std::fstream file;
    config_match conf;
    struct pollfd *client;
    size_t *timeout;
    int type;


    int state;
    bool is_cgi;
	t_cgi_data	cgi_data;

    void GET_handler();
    void GET_open_input();
    void GET_body();
    void GET_list_directory();
    void GET_check_state();

    void POST_handler();
    void POST_check_state();
    void POST_upload_normal_handler();
    void POST_upload_chunked_handler();
    void POST_check_cgi();

    void DELETE_check_state();


    void CGI_handler();
    void CGI_executer();
    void CGI_WAITER();
    void CGI_PARSER();
    int check_cgi(std::string &index);


    void SEND_handler();
    void ERROR_handler(int x);


    size_t remaining_bytes();
    public:
        http_response(http_request *req,struct pollfd *fd,std::string &host,std::string &port,size_t *out_time);
        ~http_response();
        void generate_response(pollfd *fd,http_request *req,size_t *out_time);
};

std::string int_to_string(int a);
std::string extention(std::string &file);
int count_nl (char *str,int len);
size_t getSize(std::string filename);
size_t	get_running_time(size_t start_time);
size_t	get_time(void);
std::string get_query(std::string &path);
void decode(std::string &str);