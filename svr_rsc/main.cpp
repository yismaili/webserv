/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili <yismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:21:09 by yismaili          #+#    #+#             */
/*   Updated: 2023/03/30 17:18:08 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_server.hpp"
#include <vector>


int main() {
    std::vector<int> vtr;
    int i = 10;
    while(i < 100){
         vtr.push_back(8000 + i);
         i++;
    }
  
    http::http_sever server1(vtr, "127.0.0.1");
    server1.run();
    return 0;
}
