#include "location.hpp"
#include "../include/http_server.hpp"
# include "../respond/respond.hpp"
# include "../request/request.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: could not open config file.\n";
        return (1);
    }
    (void)av;
    std::vector<server> servers;
    std::vector<int> all_ports;
    servers = ft_fill_servers(av);
    all_ports = get_all_ports(servers);
//    for (size_t i = 0; i < servers.size(); i++)
//    {
//         servers[i].display_sever();
//         std::cout << "locations :::::::::::::::::::::::::::::::::::::::::::: \n";
//         for (size_t j = 0; j < servers[i]._location.size(); j++)
//         {
//             std::cout << "location : " << servers[i]._location[j].location_name << std::endl;
//             servers[i]._location[j].display_sever();
//         }
//         std::cout << "++++++++++++++++++++++\n";
//    }  
//     for (size_t i = 0; i < all_ports.size(); i++)
//             std::cout << all_ports[i] << "\n";
    // http::http_sever server1(all_ports, "127.0.0.1");
    // server1.run();
        std::string s;
    // i need to fill it with the request 
    s = "POST /cgi_bin/test.py HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0\r\n"
    "Accept: application/json\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate, br\r\n"
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Content-Length: 34\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
    "{\"username\": \"user\", \"password\": \"pass\"}";

    request r(s);


    Respond resp(r);
    resp.response_root(servers);
return (0);
}