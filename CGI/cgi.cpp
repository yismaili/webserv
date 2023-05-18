#include "cgi.hpp"
std::map<std::string, std::string>get_env(char *file, request req)
{

    std::map<std::string, std::string> env;

    env["SERVER_SOFTWARE"] = "MyServer/1.0";
    env["SERVER_NAME"] = "localhost";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = "80";
    env["REQUEST_METHOD"] = req.get_method();
    env["SCRIPT_NAME"] = "/cgi-bin/mycgi";
    env["CONTENT_TYPE"] = req.get_header("Content-Type");
    env["CONTENT_LENGTH"] = std::to_string(req.get_body().size());
    env["QUERY_STRING"] = req.get_query();
    env["REMOTE_ADDR"] = "127.0.0.1";
    env["REMOTE_HOST"] = "localhost";
    env["REDIRECT_STATUS"] = "200";
    env["HTTP_COOKIE"] = req.get_header("Cookie");
    env["SCRIPT_FILENAME"] = file;


    return env;
}

void put_cookie(std::string output, Respond &res)
{
    std::string token;
    std::size_t pos = output.find("Set-Cookie:");
    if (pos != std::string::npos)
    {
        pos += 12;
        std::size_t end = output.find(";", pos);
        if (end != std::string::npos) {
            token = output.substr(pos);
        } else {
            token = output.substr(pos);
        }
        res.set_header("Set-Cookie", token);
    }
}

void set_headers_cgi(std::string output, Respond &res) {
    std::stringstream ss(output);
    std::string line;
    std::string body;
    bool headers_finished = false;

    put_cookie(output, res);
    body =+ "<!DOCTYPE html>";
    while (std::getline(ss, line)) {
        if (line == "\r") {
            headers_finished = true;
            continue;
        }
        if (!headers_finished) {
            std::size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);
                if(key != "Set-Cookie")
                    res.set_header(key, value);
            }
        } 
        else 
            body += line + "\n";
    }
    res.set_response_body(body);
}


std::string run_cgi(request &r,  Respond &res)
{
    char *file, *path;
    file = strdup(res.get_file_cgi().c_str());
    path = strdup(res.get_path_info_founded().c_str());

    char *cmd[3] = {path, file, NULL};
    std::string cgi_str;
    FILE *temp = std::tmpfile();
    FILE *temp1 = std::tmpfile();
    int fdtemp = fileno(temp);
    int fdtemp1 = fileno(temp1);

    std::map<std::string, std::string> env = get_env(file, r);
    std::string method = env["REQUEST_METHOD"];
    std::string content_type = env["CONTENT_TYPE"];
    std::string content_length_str = env["CONTENT_LENGTH"];
    std::string query_string = env["QUERY_STRING"];

    char **envp = (char **)malloc(sizeof(char *) * (env.size() + 1));
    int i = 0;
    for (std::map<std::string , std::string>::iterator it = env.begin(); it != env.end(); it++, i++) 
    {
        std::string e = it->first + "=" + it->second;
        envp[i] = strdup(e.c_str());
    }
    envp[i] = NULL;
    int pid = fork();
    if (pid == -1) 
    {
        res.set_status_code(500);
        res.set_status_message(res.get_response_status(res.get_status_code()));
        return NULL;
    }
    else if (pid == 0)
    {
        if (method == "POST")
        {
            if (dup2(fdtemp1, STDIN_FILENO) == -1)
            {
                std::cerr << "Error: failed to redirect stdin\n";
                exit(1);
            }
            std::fprintf(temp1, "%s", r.get_body().c_str());
            std::rewind(temp1);
        }
        if (dup2(fdtemp, STDOUT_FILENO) == -1)
        {
            std::cerr << "Error: failed to redirect stdout\n";
            exit(1);
        }
        alarm(2);
        execve(cmd[0], cmd, envp);
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) || status != 0)
    {
        res.set_status_code(500);
        res.set_status_message(res.get_response_status(res.get_status_code()));
    }
    char buf[1];
    std::string content;
    int byt;
    std::rewind(temp);
    while ((byt = read(fdtemp, buf, 1)) > 0){
        content.append(buf, 1);
    }
    //std::cout << content <<std::endl;
    //puts("heeeeere");
    if (byt == -1)
        std::cerr << "Error: failed to read output\n";
    close(fdtemp);
    close(fdtemp1);
    set_headers_cgi(content, res);
    return (content);
}
