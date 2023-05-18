#ifndef CGI_H
#define CGI_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include "../request/request.hpp"
#include "../respond/respond.hpp"

class Respond;

std::string run_cgi(request &r,  Respond &res);

#endif