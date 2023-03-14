#include "http_response.hpp"




http_response::http_response(http_request &req,int fd,config *con) : request(req)
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
    std::string methods[3] = {"GET","POST","DELETE"};

    int Types[3] = {GET,POST,DELETE};
    for (int i = 0;i < 3;i++)
    {
        type = Types[i];
        if (methods[i] == request.get_method())
            break;
    }
    check_state();
    client_fd = fd;
}

void http_response::check_state()
{
    struct stat s;

    std::cout << "/Users/hait-moh/Desktop/webserv/webserve" + request.get_path() << std::endl;
    if (stat(("/Users/hait-moh/Desktop/webserv/webserve" + request.get_path()).c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
            state = DIRECTORY;
        else
            state = FILE;
    }
    else
        throw 200;
    printf("blan akhor\n");
}


void http_response::generate_response()
{
    void (http_response::*handlers[4])() = {&http_response::GET_handler,&http_response::POST_handler,&http_response::DELETE_handler,&http_response::SEND_handler};

    (this->*handlers[type])();
}


void http_response::SEND_handler()
{
    int ret;
    if (res_header != "")
        ret = send(client_fd,res_header.c_str(),res_header.size(),0);
    else
        ret = send(client_fd,body.c_str(),body.size(),0);
    if (ret == -1)
    {
        std::cout << "error1234" << std::endl;
        return;
    }
    if (res_header != "")
        res_header.erase(0,ret);
    else
        body.erase(0,ret);
    if (body == "")
        throw 200;
}

std::string int_to_string(int a)
{
    std::stringstream temp;
    temp << a;
    return temp.str();
}
