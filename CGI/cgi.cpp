#include "cgi.hpp"
std::map<std::string, std::string>get_env(char *file, request req)
{
    std::map<std::string, std::string> env;

    env["REQUEST_METHOD"] = req.get_method();
    env["CONTENT_TYPE"] = req.get_header("Content-Type");
    env["CONTENT_LENGTH"] = std::to_string(req.get_body().size());
    env["QUERY_STRING"] = req.get_query();
    env["REDIRECT_STATUS"] = "200";
    env["HTTP_COOKIE"] = req.get_header("Cookie");
    env["SCRIPT_FILENAME"] = file;
    return env;
}


void set_headers_cgi(std::string output, Respond &res)
{
    std::stringstream ss(output);
    std::string line;
    std::string body;
    bool headers_finished = false;

    body =+ "<!DOCTYPE html>\n";
    while (std::getline(ss, line)) {
        if (line == "\r") {
            headers_finished = true;
            continue;
        }
        if (!headers_finished) {
            std::size_t pos = line.find(": ");
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);
                res.set_header(key, value);
            }
        } 
        else 
            body += line + "\n";
    }
    res.set_response_body(body);
}

void free_all(char *file, char *path, char **env, int size)
{
    for (int i = 0; i < size; i++) 
        free(env[i]);
    free (env);
    free (file);
    free (path);
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

    char **envp = (char **)malloc(sizeof(char *) * (env.size() + 1));
    if(!envp)
    {
        res.set_status_code(500);
        res.set_response_body(res.get_response_status(res.get_status_code()));
        free_all(file, path,envp, env.size());
        return res.get_response_status(res.get_status_code());
    }
    int i = 0;
    for (std::map<std::string , std::string>::iterator it = env.begin(); it != env.end(); it++) 
    {
        std::string e = it->first + "=" + it->second;
        envp[i++] = strdup(e.c_str());
    }
    envp[i] = NULL;
    int pid = fork();
    if (pid == -1) 
    {
        res.set_status_code(500);
        res.set_response_body(res.get_response_status(res.get_status_code()));
        free_all(file, path,envp, env.size());
        return NULL;
    }
    else if (pid == 0)
    {
            std::cout << r.get_body() << "\n";
        if (method == "POST")
        {

            if (dup2(fdtemp1, STDIN_FILENO) == -1)
            {
                res.set_status_code(500);
                res.set_response_body(res.get_response_status(res.get_status_code()));
                free_all(file, path,envp, env.size());
                return res.get_response_status(res.get_status_code());
            }
            write(fdtemp1, r.get_body().c_str(), r.get_body().size());
            std::rewind(temp1);
        }
        if (dup2(fdtemp, STDOUT_FILENO) == -1)
        {
            res.set_status_code(500);
            res.set_response_body(res.get_response_status(res.get_status_code()));
            free_all(file, path,envp, env.size());
            return res.get_response_status(res.get_status_code());
        }
        alarm(1);
        execve(cmd[0], cmd, envp);
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) || status != 0)
    {
        res.set_status_code(500);
        res.set_response_body(res.get_response_status(res.get_status_code()));
        free_all(file, path,envp, env.size());
        return res.get_response_status(res.get_status_code());
    }
    char buf[1];
    std::string content;
    int byt;
    std::rewind(temp);
    while ((byt = read(fdtemp, buf, 1)) > 0){
        content.append(buf, 1);
    }
    if (byt == -1)
    {
        res.set_status_code(500);
        res.set_response_body(res.get_response_status(res.get_status_code()));
        free_all(file, path,envp, env.size());
        return res.get_response_status(res.get_status_code());
    }
    close(fdtemp);
    close(fdtemp1);
    free_all(file, path,envp, env.size());
    set_headers_cgi(content, res);
    return (content);
}
