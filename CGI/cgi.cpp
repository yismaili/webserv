#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include "/Users/snouae/Desktop/server/request/request.hpp"


    // env["SERVER_SOFTWARE"] = "=webserv/1.0";
    // env["SERVER_NAME"] = "=localhost"; //get_servername
    // env["GATEWAY_INTERFACE"] = "=CGI/1.1";
    // env["SERVER_PROTOCOL"] = "=HTTP/1.1";
    // env["SERVER_PORT"] = "=80"; //getpor
    // env["REQUEST_METHOD"] = "=GET"; //
    // env["CONTENT_TYPE"] = "=who cares"; //
    // env["CONTENT_LENGTH"] = "=554"; //
    // env["QUERY_STRING"] = "=kdhfkdjf?kdfkfh?"; //
    // env["REDIRECT_STATUS"] = "=200";
    // env["DOCUMENT_ROOT"] = "=html";
    // env["SCRIPT_FILENAME"] = "=??";
    // env["HTTP_COOKIE"] = "=Cookie";
    /*	this->_env.push_back("SERVER_SOFTWARE=webserv/1.0");
	this->_env.push_back("SERVER_NAME=" + this->_location.getServerName());
	this->_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	this->_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	this->_env.push_back("SERVER_PORT= " + req.getPort());
	this->_env.push_back("REQUEST_METHOD=" + req.getMethod());
	this->_env.push_back("CONTENT_TYPE=" + req.getHeadr("Content-Type"));
	this->_env.push_back("CONTENT_LENGTH=" + toStr(req.getBody().size()));
	this->_env.push_back("QUERY_STRING=" + req.getQuery());
	this->_env.push_back("REDIRECT_STATUS=200");
	this->_env.push_back("DOCUMENT_ROOT=" + this->_location.getRoot());
	this->_env.push_back("SCRIPT_FILENAME=" + this->_cmd[1]);
	this->_env.push_back("HTTP_COOKIE=" + req.getHeadr("Cookie"));*/
std::map<std::string, std::string>get_env()
{
    //std::map<std::string, std::string> env;

    // env["SERVER_SOFTWARE"] = "MyServer/1.0";
    // env["SERVER_NAME"] = "localhost";
    // env["GATEWAY_INTERFACE"] = "CGI/1.1";
    // env["SERVER_PROTOCOL"] = "HTTP/1.1";
    // env["SERVER_PORT"] = "80";
    // // env["REQUEST_METHOD"] = req.get_method();
    // //env["SCRIPT_NAME"] = "/cgi-bin/mycgi";
    // // env["CONTENT_TYPE"] = req.get_header("Content-Type");
    // // env["CONTENT_LENGTH"] = std::to_string(req.get_body().size());
    // // env["QUERY_STRING"] = req.get_query();
    // env["REMOTE_ADDR"] = "127.0.0.1";
    // env["REMOTE_HOST"] = "localhost";
    // env["REDIRECT_STATUS"] = "200";
    // env["HTTP_COOKIE"] = req.get_header("Cookie");
    std::map<std::string, std::string> env;

    env["SERVER_SOFTWARE"] = "MyServer/1.0";
    env["SERVER_NAME"] = "localhost";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = "80";
    env["REQUEST_METHOD"] = "GET";
    env["SCRIPT_NAME"] = "/cgi-bin/mycgi";
    env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
    env["CONTENT_LENGTH"] = "0";
    env["QUERY_STRING"] = "";

    // Parse QUERY_STRING
    // std::string query_string = getenv("QUERY_STRING");
    // if (!query_string.empty())
    // {
    //     std::vector<std::string> tokens;
    //     std::istringstream iss(query_string);
    //     std::string token;
    //     while (std::getline(iss, token, '&'))
    //     {
    //         tokens.push_back(token);
    //     }

    //     for (const auto& t : tokens)
    //     {
    //         std::vector<std::string> param;
    //         std::istringstream iss(t);
    //         std::string p;
    //         while (std::getline(iss, p, '='))
    //         {
    //             param.push_back(p);
    //         }

    //         if (param.size() == 2)
    //         {
    //             env[param[0]] = param[1];
    //         }
    //     }
    // }

    env["REMOTE_ADDR"] = "127.0.0.1";
    env["REMOTE_HOST"] = "localhost";
    env["REDIRECT_STATUS"] = "200";
    return env;
}


std::string run_cgi()
{
    char *cmd[3] = {"/usr/bin/python3", "test.py", NULL};
    std::string cgi_str;

    FILE *temp = std::tmpfile();
    FILE *temp1 = std::tmpfile();
    int fdtemp = fileno(temp);
    int fdtemp1 = fileno(temp1);

    std::map<std::string, std::string> env = get_env();
    std::string method = env["REQUEST_METHOD"];
    std::string content_type = env["CONTENT_TYPE"];
    std::string content_length_str = env["CONTENT_LENGTH"];
    std::string query_string = env["QUERY_STRING"];

    char **envp = (char **)malloc(sizeof(char *) * (env.size() + 1));
    int i = 0;
    for (std::map<std::string , std::string>::iterator it = env.begin(); it != env.end(); ++it, ++i) 
    {
        std::string e = it->first + "=" + it->second;
        envp[i] = strdup(e.c_str());
    }
    envp[i] = NULL;
    int pid = fork();
    if (pid == -1) 
    {
        std::cerr << "Error: failed to fork process\n";
        exit(1);
    }
    else if (pid == 0)
    {
        if (method == "POST")
        {
            std::fprintf(temp1, "%s", "name=John&email=john@example.com");
            std::rewind(temp1);
            if (dup2(fdtemp1, STDIN_FILENO) == -1)
            {
                std::cerr << "Error: failed to redirect stdin\n";
                exit(1);
            }
        }
        if (dup2(fdtemp, STDOUT_FILENO) == -1)
        {
            std::cerr << "Error: failed to redirect stdout\n";
            exit(1);
        }
        execve(cmd[0], cmd, envp);
        //std::cerr << "Error: failed to execute command\n";
        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);
    if (status != 0)
        std::cerr << "error\n";
    char buf[1];
    std::string content;
    int byt;
    std::rewind(temp);
    while ((byt = read(fdtemp, buf, 1)) > 0){
        content.append(buf, 1);
    }
    if (byt == -1)
        std::cerr << "Error: failed to read output\n";
    //dup2(STDOUT_FILENO, fdtemp);
    close(fdtemp);
    close(fdtemp1);
    return (content);
}

int main()
{
    std::cout << run_cgi() << "\n";
}
