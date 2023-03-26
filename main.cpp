/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/25 16:53:50 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_server.hpp"

int main() {
    http::tcpServer server1(8080, "0.0.0.0");
    //  http::tcpServer server2(80, "0.0.0.0");
    //   http::tcpServer server3(90, "0.0.0.0");
    //    http::tcpServer server4(160, "0.0.0.0");
    server1.run();
    //  server2.run();
    //   server3.run();
    //    server4.run();
    return 0;
}
