/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/22 15:13:29 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpTcpServer.hpp"

int main() {
    http::tcpServer server(8080);
    if (!server.Start()) {
        std::cerr << "Error starting server" << std::endl;
        return 1;
    }
    server.Run();
    return 0;
}
