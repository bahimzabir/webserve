#include "http_response.hpp"




http_response::http_response(http_request &req,struct pollfd *fd) : request(req)
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
    std::map<std::string,int> met_map;
    met_map["GET"] = GET;
    met_map["POST"] = POST;
    met_map["DELETE"] = DELETE;
    
    if (met_map.find(request.get_method()) == met_map.end())
        throw NOT_IMPLEMENTED;
    if (std::find(conf.methods.begin(),conf.methods.end(),request.get_method()) == conf.methods.end())
        throw NOT_ALLOWED;
    type = met_map[request.get_method()];
    if (state == GET || state == DELETE)
        client->events = POLLIN;
    check_state();
    client = fd;
}

void http_response::check_state()
{
    struct stat s;

    std::cout << conf.root << std::endl;
    if (stat(conf.root.c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
        {
            for (int i = 0;i < conf.index.size();i++)
            {
                std::ifstream f(conf.root + "/" + conf.index[i]);
                if (f.good())
                {
                    state = FILE;
                    status[1] = "/" + conf.index[i];
                    f.close();
                    return;
                }
                f.close();
            }
            if (conf.autoindex)
                state = LIST_DIRECTORY;
            else
                throw 403;
        }
        else
            state = FILE;
    }
    else
        throw 404;
}


void http_response::generate_response()
{
    void (http_response::*handlers[4])() = {&http_response::GET_handler,&http_response::POST_handler,&http_response::DELETE_handler,&http_response::SEND_handler,};
    try
    {
        (this->*handlers[type])();
    }
    catch(int x)
    {
        ERROR_handler(x);
    }
}


void http_response::SEND_handler()
{
    int ret;
    if (res_header != "")
        ret = send(client->fd,res_header.c_str(),res_header.size(),0);
    else
        ret = send(client->fd,body.c_str(),body.size(),0);
    if (ret == -1)
    {
        throw 666;
    }
    if (res_header != "")
        res_header.erase(0,ret);
    else
        body.erase(0,ret);
    if (body == "")
        throw 666;
}

void http_response::ERROR_handler(int x)
{
    res_header = "";
    headers.clear();
    body = "";
    std::ifstream input;

    /*if (conf.error_pages.count(x))
    {
        state = FILE;
        if ( conf.error_pages[x] == exists)
            use it;

    }
    else
        use normal one;*/
    error_pages_map errors;
    body += errors[x];
    headers["Content-Length"] = int_to_string(body.size());
    headers["Content-Type"] = content_type["html"];
    res_header += "HTTP/1.1 200 OK\n";
    for (std::map<std::string,std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        res_header +=  (*it).first + ":" + (*it).second + "\n";
    type = SEND;
}                                          

std::string int_to_string(int a)
{
    std::stringstream temp;
    temp << a;
    return temp.str();
}
