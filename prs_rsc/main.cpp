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
    std::vector<server> servers;
    std::vector<int> all_ports;
    servers = ft_fill_servers(av);
    all_ports = get_all_ports(servers);
//    for (size_t i = 0; i < servers.size(); i++)
//    {
//        servers[i].display_sever();
//         std::cout << "locations :::::::::::::::::::::::::::::::::::::::::::: \n";
//         for (size_t j = 0; j < servers[i]._location.size(); j++)
//         {
//             std::cout << "location : " << servers[i]._location[j].location_name << std::endl;
//             servers[i]._location[j].display_sever();
//             std::cout << ":::::::::::::::::::::::::::::::::::::::::::: \n";
//         }
//         std::cout << "++++++++++++++++++++++\n";
//    }  
//     for (size_t i = 0; i < all_ports.size(); i++)
//             std::cout << all_ports[i] << "\n";
    http::http_sever server1(servers);
    server1.run();
return (0);
}