#include "../http_response.hpp"

std::string cgii::getScript(const std::string &uri) {
	std::string script;
	std::string::size_type pos = uri.find('?');
	if (pos != std::string::npos)
		script = uri.substr(0, pos);
	else
		script = uri;
	return script;
}

std::string cgii::getQueries(const std::string &uri) {
	std::string queries;
	std::string::size_type pos = uri.find('?');
	if (pos != std::string::npos)
		queries = uri.substr(pos + 1);
	return queries;
}



void	http_response::CGI_handler() {
	std::cout << "Wach dekhlat?" << std::endl;
	int i = 0;
	int status, pid, fd;
	int p[2];
	char *args[3];
	char *env[2];

	char *pwd = getcwd(NULL, 0);
	std::string pathInfo = std::string(pwd);
	env[0] = strdup(("PATH_INFO=" + pathInfo).c_str());
	env[1] = NULL;
	std::cout << env[i++] << std::endl;
	free(pwd);
	// fd = open(cgi_data.input, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	fd = cgi_data.input_fd;
	int out_fd = cgi_data.output_fd;
	pid = fork();
	if (pid == 0) {
		dup2(fd, 0);
		dup2(out_fd, 1);
		close(fd);
		args[0] = strdup(conf.cgi_pass[0].cgi_param.c_str());
		args[1] = strdup(conf.root.c_str());
		args[2] = NULL;
		if (execve(args[0], args, env) == -1) {
			std::cout << "execve error" << std::endl;
			exit(1);
		}
	}
	else {
		waitpid(pid, &status, 0);
		close(fd);
	}
}