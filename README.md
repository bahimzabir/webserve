# Custom HTTP Server Project

This project is a custom HTTP server implementation that allows you to serve static websites, handle various HTTP methods, manage routes, execute CGI scripts, and more. It was developed as part of [École 42](https://www.42.fr/) curriculum.

## Features

- **Custom Configuration**: Configure server parameters, ports, hosts, error pages, and more using a configuration file.
- **Non-blocking I/O**: The server operates in a non-blocking manner using `poll()` for efficient I/O handling.
- **HTTP Methods**: Supports essential HTTP methods including GET, POST, and DELETE.
- **Routing**: Define routes with various configurations such as redirection, file serving, CGI execution, and more.
- **Static Website**: Fully capable of serving static websites and generating directory listings.
- **CGI Execution**: Execute CGI scripts based on specified file extensions.
- **Cookies and Sessions**: Supports basic cookies and session management.
- **Multi-CGI Support**: Capable of handling multiple CGI scripts.

## Getting Started

1. **Clone the Repository**: `git clone https://github.com/yourusername/your-http-server.git`
2. **Build the Server**: `make`
3. **Run the Server**: `./webserv config.conf`
4. **Access Your Site**: Open your web browser and navigate to `http://localhost:port`

## Configuration

To configure the server, modify the `config.conf` file. Customize port, host, routes, CGI settings, error pages, and more.

## Routes Configuration

Routes can be defined in the configuration file. Configure accepted methods, redirections, file serving, CGI execution, and more for each route.

## Example Usage

Here's an example of a simple configuration in `config.conf`:

```plaintext
server
{	
    host         127.0.0.1;
    listen       8000;
    server_name  name1;
    max_client_body_size 20000000;

    location / { 
        allow_methods GET POST;
        autoindex 1;
        index index.html;

    }

    location /dir { 
        allow_methods GET POST DELETE;
        index index.html index.php;
        autoindex 1;
        root /Users/azabir/Desktop/42Cursus/webserve;
        cgi_pass pl /usr/bin/perl;
        cgi_pass php /Users/azabir/Desktop/42Cursus/webserve/php-cgi;
    }
}
```
