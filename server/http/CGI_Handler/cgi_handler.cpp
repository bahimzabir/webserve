#include "../http_response.hpp"

void	http_response::CGI_executer() {
	int i = 0;
	int status, pid, fd;
	int p[2];
	char *args[3];
	char *env[10];

	char *pwd = getcwd(NULL, 0);
	std::string pathInfo = std::string(pwd);
    int len = 0;
    if (request->get_header("TRANSFER-ENCODING") == "chunked")
        len = getSize(cgi_data.input);
    else
        len = std::atoi(request->get_header("CONTENT-LENGTH").c_str());
    if (request->get_header("CONTENT-TYPE") != "")
        env[0] = strdup(("CONTENT_TYPE=" + request->get_header("CONTENT-TYPE")).c_str());
    else
        env[0] = strdup(("CONTENT_TYPE=NULL"));
    
    env[1] = strdup(("REQUEST_METHOD=" + request->get_method()).c_str());
    env[2] = strdup(("CONTENT_LENGTH=" + int_to_string(len)).c_str());
    env[3] = strdup(("SCRIPT_FILENAME=" + conf.root).c_str());
    env[4] = strdup("REDIRECT_STATUS=200");
	env[5] = NULL;
	free(pwd);
	fd = cgi_data.input_fd;
	int out_fd = cgi_data.output_fd;
	pid = fork();
	if (pid == 0) {
        if (cgi_data.input != "")
		    dup2(fd, 0);
		dup2(out_fd, 1);
		close(fd);
		args[0] = strdup(conf.cgi_pass[0].cgi_param.c_str());
		args[1] = strdup(conf.root.c_str());
		args[2] = NULL;
		execve(args[0], args, env);
		exit(1);
	}
    else if (pid == -1)
        throw SERVER_ERROR;
    cgi_data.pid = pid;
    state = WAITER;
    free(env[0]);
}


void	http_response::CGI_WAITER() {
    int status = -1;
    int ret = waitpid(cgi_data.pid,&status,WNOHANG);
    if (WEXITSTATUS(status) == 1)
        throw BAD_GATEWAY;
    if (ret != 0)
    {
        state = PARSER;
        if (cgi_data.input != "")
            close(cgi_data.input_fd);
        close(cgi_data.output_fd);
        file.open(cgi_data.output);
        if (!file.good())
        {
            std::cout << "---------" << cgi_data.output <<std::endl;
            throw SERVER_ERROR;
        }
        request->reset();
    }
}

long long http_response::remaining_bytes()
{
    std::streampos begin = file.tellg();
    file.seekg(0,std::ios::end);
    std::streampos end = file.tellg();
    file.seekg(begin);
    return end - begin;
}

void	http_response::CGI_PARSER() {
    char buffer[4000];
    error_pages_map m;
    std::stringstream rem;
    file.read(buffer,1000);

    buffer[file.gcount()] = 0;
    request->parse_remaining(buffer,file.gcount(),count_nl(buffer,file.gcount()));
    if (request->get_state() == REQUEST_BODY)
    {
        type = GET;
        state = RESPONSE_BODY;
        if (request->get_header("STATUS") != "")
            res_header += "HTTP/1.1 " + request->get_header("STATUS") + "\n";
        else if (request->get_header("LOCATION") != "")
            res_header += "HTTP/1.1 301 Moved Permanently\n";
        else
            res_header += "HTTP/1.1 200 OK\n";
        res_header += request->get_headers();
        request->get_remaining().read(buffer,4000);
        body.append(buffer,request->get_remaining().gcount());
        if (request->get_header("CONTENT-LENGTH") == "")
        {
            res_header += "Content-length: " + int_to_string(remaining_bytes() + body.size()) + "\n";
            content_remaining = remaining_bytes() + body.size();
        }
        else
        {
            res_header += "Content-length: " + request->get_header("CONTENT-LENGTH") + "\n";
            content_remaining = std::atoi(request->get_header("CONTENT-LENGTH").c_str());
        }
        res_header += "\n";
        return;
    }
    if (file.eof())
        throw BAD_GATEWAY;
    if (!file.good())
        throw SERVER_ERROR;
}
void	http_response::CGI_handler() {
    is_cgi = 1;
    if (state == EXECUTOR)
        CGI_executer();
    else if (state == WAITER)
        CGI_WAITER();
    else if (state == PARSER)
        CGI_PARSER();
}