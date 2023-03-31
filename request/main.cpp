#include "request.hpp"

int main(void)
{
    // i have to test request.cpp through the main function so i need to pass the request to it
    // s string need to read all the argv[i] of the request
    std::string s;
    // i need to fill it with the request 
s = "POST /api/v1/login HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0\r\n"
    "Accept: application/json\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate, br\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 34\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
    "{\"username\": \"user\", \"password\": \"pass\"}";

    request r(s);
    return (0);
}