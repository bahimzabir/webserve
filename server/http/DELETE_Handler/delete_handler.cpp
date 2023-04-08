#include "../http_response.hpp"

void http_response::DELETE_check_state()
{
    client->events = POLLOUT;
    struct stat s;
    if (stat(conf.root.c_str(),&s) == 0)
    {
        if (S_ISDIR(s.st_mode))
        {
            if (access(conf.root.c_str(),W_OK))
                throw FORBIDDEN;
            DIR *directory;
            struct dirent *d;
            directory = opendir(conf.root.c_str());
            if (!directory)
                throw FORBIDDEN;
            d = readdir(directory);
            while (d)
            {
                std::string path = d->d_name;
                if (path != "." && path != ".." && access((conf.root + "/" + path).c_str(),W_OK))
                    throw FORBIDDEN;
                d = readdir(directory);
            }
            rewinddir(directory);
            d = readdir(directory);
            while (d)
            {
                std::string path = d->d_name;
                if (path != "." && path != ".." && remove((conf.root + "/" + path).c_str()))
                    throw SERVER_ERROR;
                d = readdir(directory);
            }
            if (rmdir(conf.root.c_str()))
                throw SERVER_ERROR;
            throw NO_CONTENT;
        }
        else
        {
            if (access((conf.root).c_str(),W_OK))
                throw FORBIDDEN;
            if (remove(conf.root.c_str()))
                throw SERVER_ERROR;
            throw NO_CONTENT;
        }
    }
    else
        throw NOT_FOUND;
}