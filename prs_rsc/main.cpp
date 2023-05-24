#include "location.hpp"
#include "../include/http_server.hpp"
# include "../respond/respond.hpp"
# include "../request/request.hpp"

int main(int ac, char **av)
{
    if (ac > 2)
    {
        std::cerr << "Error: could not open config file.\n";
        return (1);
    }
    // Register a custom signal handler for demonstration
    std::vector<server> servers;
    std::vector<int> all_ports;
    servers = ft_fill_servers(av, ac);
    all_ports = get_all_ports(servers);
    //     for (size_t i = 0; i < all_ports.size(); i++)
    //             std::cout << all_ports[i] << "\n";
    http::http_sever server1(servers);
    server1.run();
return (0);
}