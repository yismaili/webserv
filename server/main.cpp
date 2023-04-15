/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/04/14 23:07:36 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/http_server.hpp"
#include <vector>
int main() {
    std::vector<int> vtr;
    //int i = 10;
   // while(i < 100){
         vtr.push_back(8080);
        //   vtr.push_back(8000);
        //   vtr.push_back(8090);
        // i++;
    //}
    http::http_sever server1(vtr, "127.0.0.1");
    server1.run();
    return 0;
}
