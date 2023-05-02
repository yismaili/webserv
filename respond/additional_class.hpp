#include "respond.hpp"

class FormData
{
    public:
        std::string name;
        std::string content_type;
        std::string data;
        std::string file_name;
        bool isValid() const
        {
            return (!name.empty() && !data.empty());
        }
};
class Url_encoded
{
    public:
        std::string key;
        std::string value;
};