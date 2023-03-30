/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/30 16:13:42 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_server.hpp"
#include <vector>


int main() {
    std::vector<int> vtr;
    
    vtr.push_back(8080);
    vtr.push_back(8090);
    vtr.push_back(8070);
    vtr.push_back(8060);
    vtr.push_back(8050);
    vtr.push_back(8040);
    http::http_sever server1(vtr, "127.0.0.1");
    server1.run();
    return 0;
}
