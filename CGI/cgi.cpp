#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>


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
std::map<std::string, std::string>get_env()
{
    std::map<std::string, std::string> env;

    env["SERVER_SOFTWARE"] = "MyServer/1.0";
    env["SERVER_NAME"] = "localhost";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = "80";
    env["REQUEST_METHOD"] = "GET";
    env["SCRIPT_NAME"] = "/cgi-bin/mycgi";
    env["CONTENT_TYPE"] = "text/html";
    env["CONTENT_LENGTH"] = "";
    env["QUERY_STRING"] = "";
    env["REMOTE_ADDR"] = "127.0.0.1";
    env["REMOTE_HOST"] = "localhost";
    env["REDIRECT_STATUS"] = "200";
    return (env);
}


std::string run_cgi(request &req)
{
    char *cmd[3] = {"python.py", "/usr/bin/python3", NULL};
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
            std::fprintf(temp1, "%s", req.body().c_str());
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
        std::cerr << "Error: failed to execute command\n";
        exit(1);
    }

    close(fdtemp);
    close(fdtemp1);

    int status;
    waitpid(pid, &status, 0);
    if (status != 0)
        std::cerr << "error";
    char buf;
    while ((nbytes = read(fdtemp, buf, 1)) > 0)
        content.append(buf, 1);
    if (nbytes == -1)
        std::cerr << "Error: failed to read output\n";
    return (content);
}


