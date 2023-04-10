#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>


std::map<std::string, std::string>get_env()
{
    std::map<std::string, std::string> env;
    env["SERVER_SOFTWARE"] = "webserv/1.0";
    env["SERVER_NAME"] = "localhost"; //get_servername
    env["GATEWAY_INTERFACE"] = "=CGI/1.1";
    env["SERVER_PROTOCOL"] = "=HTTP/1.1";
    env["SERVER_PORT"] = "=80"; //getpor
    env["REQUEST_METHOD"] = "=GET"; //
    env["CONTENT_TYPE"] = "=who cares"; //
    env["CONTENT_LENGTH"] = "=554"; //
    env["QUERY_STRING"] = "=kdhfkdjf?kdfkfh?"; //
    env["REDIRECT_STATUS"] = "=200";
    env["DOCUMENT_ROOT"] = "=html";
    env["SCRIPT_FILENAME"] = "=??";
    env["HTTP_COOKIE"] = "Cookie";

    return (env);
}


int main()
{
    // int finput;
    // int foutput;

    // finput = dup(0);
    // foutput = dup(1);

    // int pid = fork();

    // if (pid != 0)
    // {
        
    // }
    char *cmd[3];
    cmd[0] = "/usr/bin/python3";
    cmd[1] = "python.py";
    cmd[2] = NULL;

    FILE		*temp = std::tmpfile();
    FILE		*temp1 = std::tmpfile();
    int			fdtemp = fileno(temp);
    int			fdtemp1 = fileno(temp1);

    char **envp = (char **)malloc(sizeof(char *) * 15);
    envp[14] = NULL;
    std::map<std::string, std::string> env = get_env();
  std::map<std::string, std::string>::iterator it;
    int i = 0;
    for (it = env.begin(); it != env.end(); ++it) 
    {
        std::string e;
        e = it->first + it->second;
        envp[i] = (char *)e.c_str();
        i++;
    }

    int fd = fork();
    if (fd == 0)
    {
        if(dup2(fdtemp, 1) < 0);
            std::cerr << "errror\n";
        execve(cmd[0], cmd, envp);
        exit(0);
    }
}
