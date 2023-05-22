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
            return (!name.empty() && !data.empty() && !file_name.empty());
        }
        // getters
        std::string get_name() const
        {
            return (name);
        }
        std::string get_content_type() const
        {
            return (content_type);
        }
        std::string get_data() const
        {
            return (data);
        }
        std::string get_file_name() const
        {
            return (file_name);
        }
};
class Url_encoded
{
    public:
        std::string key;
        std::string value;
};