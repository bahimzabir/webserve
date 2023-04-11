#include "http_response.hpp"

http_response::http_response(http_request *req,struct pollfd *fd,std::string &host,std::string &port,size_t *out_time)
{
    content_type["html"] =  "text/html";
    content_type["htm"] =  "text/html";
    content_type["shtml"] =  "text/html";
    content_type["css"] =  "text/css";
    content_type["xml"] =  "text/xml";
    content_type["gif"] =  "image/gif";
    content_type["jpeg"] =  "image/jpeg";
    content_type["jpg"] =  "image/jpeg";
    content_type["js"] =  "application/x-javascript";
    content_type["atom"] =  "application/atom+xml";
    content_type["rss"] =  "application/rss+xml";
    content_type["mml"] =  "text/mathml";
    content_type["txt"] =  "text/plain";
    content_type["jad"] =  "text/vnd.sun.j2me.app-descriptor";
    content_type["wml"] =  "text/vnd.wap.wml";
    content_type["htc"] =  "text/x-component";
    content_type["png"] =  "image/png";
    content_type["tif"] =  "image/tiff";
    content_type["tiff"] =  "image/tiff";
    content_type["wbmp"] =  "image/vnd.wap.wbmp";
    content_type["ico"] =  "image/x-icon";
    content_type["jng"] =  "image/x-jng";
    content_type["bmp"] =  "image/x-ms-bmp";
    content_type["svg"] =  "image/svg+xml";
    content_type["svgz"] =  "image/svg+xml";
    content_type["webp"] =  "image/webp";
    content_type["jar"] =  "application/java-archive";
    content_type["war"] =  "application/java-archive";
    content_type["ear"] =  "application/java-archive";
    content_type["hqx"] =  "application/mac-binhex40";
    content_type["doc"] =  "application/msword";
    content_type["pdf"] =  "application/pdf";
    content_type["ps"] =  "application/postscript";
    content_type["eps"] =  "application/postscript";
    content_type["ai"] =  "application/postscript";
    content_type["rtf"] =  "application/rtf";
    content_type["xls"] =  "application/vnd.ms-excel";
    content_type["ppt"] =  "application/vnd.ms-powerpoint";
    content_type["wmlc"] =  "application/vnd.wap.wmlc";
    content_type["kml"] =  "application/vnd.google-earth.kml+xml";
    content_type["kmz"] =  "application/vnd.google-earth.kmz";
    content_type["7z"] =  "application/x-7z-compressed";
    content_type["cco"] =  "application/x-cocoa";
    content_type["jardiff"] =  "application/x-java-archive-diff";
    content_type["jnlp"] =  "application/x-java-jnlp-file";
    content_type["run"] =  "application/x-makeself";
    content_type["pl"] =  "application/x-perl";
    content_type["pm"] =  "application/x-perl";
    content_type["prc"] =  "application/x-pilot";
    content_type["pdb"] =  "application/x-pilot";
    content_type["rar"] =  "application/x-rar-compressed";
    content_type["rpm"] =  "application/x-redhat-package-manager";
    content_type["sea"] =  "application/x-sea";
    content_type["swf"] =  "application/x-shockwave-flash";
    content_type["sit"] =  "application/x-stuffit";
    content_type["tcl"] =  "application/x-tcl";
    content_type["tk"] =  "application/x-tcl";
    content_type["der"] =  "application/x-x509-ca-cert";
    content_type["pem"] =  "application/x-x509-ca-cert";
    content_type["crt"] =  "application/x-x509-ca-cert";
    content_type["xpi"] =  "application/x-xpinstall";
    content_type["xhtml"] =  "application/xhtml+xml";
    content_type["zip"] =  "application/zip";
    content_type["bin"] =  "application/octet-stream";
    content_type["exe"] =  "application/octet-stream";
    content_type["dll"] =  "application/octet-stream";
    content_type["deb"] =  "application/octet-stream";
    content_type["dmg"] =  "application/octet-stream";
    content_type["eot"] =  "application/octet-stream";
    content_type["iso"] =  "application/octet-stream";
    content_type["img"] =  "application/octet-stream";
    content_type["msi"] =  "application/octet-stream";
    content_type["msp"] =  "application/octet-stream";
    content_type["msm"] =  "application/octet-stream";
    content_type["mid"] =  "audio/midi";
    content_type["midi"] =  "audio/midi";
    content_type["kar"] =  "audio/midi";
    content_type["mp3"] =  "audio/mpeg";
    content_type["ogg"] =  "audio/ogg";
    content_type["m4a"] =  "audio/x-m4a";
    content_type["ra"] =  "audio/x-realaudio";
    content_type["3gpp"] =  "video/3gpp";
    content_type["3gp"] =  "video/3gpp";
    content_type["mp4"] =  "video/mp4";
    content_type["mpeg"] =  "video/mpeg";
    content_type["mpg"] =  "video/mpeg";
    content_type["mov"] =  "video/quicktime";
    content_type["webm"] =  "video/webm";
    content_type["flv"] =  "video/x-flv";
    content_type["m4v"] =  "video/x-m4v";
    content_type["mng"] =  "video/x-mng";
    content_type["asx"] =  "video/x-ms-asf";
    content_type["asf"] =  "video/x-ms-asf";
    content_type["wmv"] =  "video/x-ms-wmv";
    content_type["avi"] =  "video/x-msvideo";

    content_remaining = 0;
    request = req;
    timeout = out_time;
    void (http_response::*state_handlers[3])() = {&http_response::GET_check_state,&http_response::POST_check_state,&http_response::DELETE_check_state};

    // for (int i = 0; i <  conf.methods.size(); i++) {
    //     std::cout << "method: [" << conf.methods[i] << "]\n";
    // }
    //     std::cout << "root: [" << conf.root << "]\n"; 
    // for (int i = 0; i <  conf.index.size(); i++) {
    //     std::cout << "index: [" << conf.index[i] << "]\n";
    // }
    // std::cout << "autoindex: [" << conf.autoindex << "]\n";
    // std::cout << "upload_pass: [" << conf.upload_pass << "]\n";
    // for (int i = 0; i <  conf.cgi_pass.size(); i++) {
    //     std::cout << "cgi_param: [" << conf.cgi_pass[i].cgi_param << "]\n";
    //     std::cout << "cgi_pass: [" << conf.cgi_pass[i].cgi_pass << "]\n";
    // }
    // std::cout << "mcbs: [" << conf.client_max_body_size << "]\n";
    // for (std::map<int, std::string>::iterator it = conf.err_pages.begin(); it != conf.err_pages.end(); it++)
    // {
    //     std::cout << "err_page: [" << it->first << "-" << it->second << "]\n";
    // }
    // std::cout << "ret_value: [" << conf.return_value << "]\n";


    // std::cerr <<"["<< host << "] [" + port + "] [" + request->get_path() + "] [" + request->get_header("HOST") + "]\n";
    // std::cerr <<"THE '/' is added to the root end, do not add it again!\n";
    // std::cout << "------------------ " << conf.root <<  "----" << std::endl;
    std::map<std::string,int> met_map;
    met_map["GET"] = GET;
    met_map["POST"] = POST;
    met_map["DELETE"] = DELETE;
    client = fd;
    client->events = POLLOUT;
    is_cgi = 0;
    try
    {
        conf.query = get_query(request->get_path());
        decode(conf.query);
        decode(request->get_path());
        get_config(host,port,request->get_path(),request->get_header("HOST"), &conf);
        if (met_map.find(request->get_method()) == met_map.end())
            throw NOT_IMPLEMENTED;
        if (conf.methods[0] != "ALL" && std::find(conf.methods.begin(),conf.methods.end(),request->get_method()) == conf.methods.end())
            throw NOT_ALLOWED;
        type = met_map[request->get_method()];
        if (conf.return_value != "")
            throw 301;
        (this->*state_handlers[type])();
        if (type == CGI && request->get_method() == "GET")
        {
            std::string output = "/tmp/XXXXXX";
            int ot_fd = mkstemp(&output[0]);
            if (ot_fd == -1)
                throw SERVER_ERROR;
            cgi_data.output = output;
            cgi_data.output_fd = ot_fd;
            state = EXECUTOR;
        }
    }
    catch(int x)
    {
        if (x == CREATED && is_cgi)
        {
            type = CGI;
            client->events = POLLOUT;
            state = EXECUTOR;
            file.clear();
            file.close();
            return;
        }
        ERROR_handler(x);
    }
}


void http_response::generate_response(pollfd *fd,http_request *req,size_t *out_time)
{
    client = fd;
    request = req;
    timeout = out_time;
    void (http_response::*handlers[4])() = {&http_response::GET_handler,&http_response::POST_handler,&http_response::CGI_handler,&http_response::SEND_handler};
    try
    {
        (this->*handlers[type])();
    }
    catch(int x)
    {
        std::cout << "ERROR   " << x << std::endl;
        if (x == CREATED && is_cgi)
        {
            type = CGI;
            client->events = POLLOUT;
            state = EXECUTOR;
            file.clear();
            file.close();
            return;
        }
        ERROR_handler(x);
    }
}


void http_response::SEND_handler()
{
    int ret;
    std::cout << client->fd << std::endl;
    if (res_header != "")
    {
        ret = send(client->fd,res_header.c_str(),res_header.size(),0);
        *timeout = get_time();
    }
    else
    {
        ret = send(client->fd,body.c_str(),body.size(),0);
        *timeout = get_time();
    }
    if (ret == -1)
        throw END;
    if (res_header == "")
        content_remaining -= ret;
    if (res_header != "")
        res_header.erase(0,ret);
    else
        body.erase(0,ret);
    if (res_header == "" && !content_remaining)
        throw END;
}

void http_response::ERROR_handler(int x)
{
    std::cout << "Error asat ::  " << x << std::endl;
    res_header.clear();
    body.clear();
    if (x != 301)
        headers.clear();
    if (file.is_open())
        file.close();
    client->events = POLLOUT;
    if (x == END)
        throw x;
    if (conf.err_pages[x] != "")
    {
        file.open(conf.err_pages[x]);
        if (file.good())
        {
            conf.root = conf.err_pages[x];
            conf.err_pages[x] = "";
            type = GET;
            state = FILE;
            file.close();
            return;
        }
        else if (file.is_open())
            file.close();
        conf.err_pages[x] = "";
    }
    error_pages_map errors;
    body += errors.get_error(x);
    content_remaining = body.size();
    headers["Content-Length"] = int_to_string(body.size());
    std::cout << "excesss        = " << body  << std::endl;
    headers["Content-Type"] = content_type["html"];
    std::cout << "************" << headers["LOCATION"] <<std::endl;
    if (x == 301 && headers["LOCATION"] == "")
        headers["LOCATION"] = conf.return_value;
    res_header = "HTTP/1.1 " + int_to_string(x) + " " + errors.get_message(x) + "\n";
    for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        res_header +=  (*it).first + ":" + (*it).second + "\n";
    res_header += '\n';
    std::cout << body.size() << " " << res_header.size() << std::endl;
    type = SEND;
}

int http_response::check_cgi(std::string &file)
{
    for (int c = 0; c < conf.cgi_pass.size();c++)
    {  
        if (extention(file) == conf.cgi_pass[c].cgi_pass)
        {
            conf.binary_root = conf.cgi_pass[c].cgi_param;
            return 1;
        }
    }
    return 0;
}

std::string int_to_string(int a)
{
    std::stringstream temp;
    temp << a;
    return temp.str();
}

size_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000 + (current_time.tv_usec) / 1000));
}

size_t	get_running_time(size_t start_time)
{
	return (get_time() - start_time);
}
std::string get_query(std::string &path)
{
    size_t b;
    std::string tmp;
    b = path.find_last_of('?');
    if (b == path.npos)
        return ("");
    if (b + 1 < path.size())
        tmp = path.substr(b + 1);
    path.erase(b,path.size());
    return tmp;
}

void decode(std::string &str)
{
    std::string decoded;
    int charac;
    for (int i = 0;i < str.size();i++)
    {
        if (str[i] == '%' && i + 2 < str.size())
        {
            charac = strtoll(str.substr(i + 1,2).c_str(),NULL,16);
            if (charac != 0)
                decoded += (char)charac;
            i += 2;
        }
        else
            decoded += str[i];
    }
    str = decoded;
}