#include "respond.hpp"

const std::pair<std::string , std::string> mime_type[] = 
{
    std::pair<std::string, std::string>("html", "text/html"),
    std::pair<std::string, std::string>("htm", "text/html"),
    std::pair<std::string, std::string>("css", "text/css"),
    std::pair<std::string, std::string>("js", "application/javascript"),
    std::pair<std::string, std::string>("gif", "image/gif"),
    std::pair<std::string, std::string>("jpg", "image/jpeg"),
    std::pair<std::string, std::string>("jpeg", "image/jpeg"),
    std::pair<std::string, std::string>("png", "image/png"),
    std::pair<std::string, std::string>("svg", "image/svg+xml"),
    std::pair<std::string, std::string>("txt", "text/plain"),
    std::pair<std::string, std::string>("xml", "text/xml"),
    std::pair<std::string, std::string>("json", "application/json"),
    std::pair<std::string, std::string>("pdf", "application/pdf"),
    std::pair<std::string, std::string>("zip", "application/zip"),
    std::pair<std::string, std::string>("rar", "application/x-rar-compressed"),
    std::pair<std::string, std::string>("7z", "application/x-7z-compressed"),
    std::pair<std::string, std::string>("mp3", "audio/mpeg"),
    std::pair<std::string, std::string>("mp4", "video/mp4"),
    std::pair<std::string, std::string>("mkv", "video/x-matroska"),
    std::pair<std::string, std::string>("avi", "video/x-msvideo"),
    std::pair<std::string, std::string>("exe", "application/x-msdownload"),
    std::pair<std::string, std::string>("msi", "application/x-msdownload"),
    std::pair<std::string, std::string>("cab", "application/vnd.ms-cab-compressed"),
    std::pair<std::string, std::string>("c", "text/x-c"),
    std::pair<std::string, std::string>("cpp", "text/x-c++"),
    std::pair<std::string, std::string>("", "text/html")
};

std::string Respond::get_mime_type(std::string file_extension)
{
    size_t mime_type_size = sizeof(mime_type) / sizeof(mime_type[0]);
    for (size_t i = 0; i < mime_type_size; i++)
    {
        if (mime_type[i].first == file_extension)
            return (mime_type[i].second);
    }
    return (mime_type[mime_type_size - 1].second);
}

const std::pair<int, std::string> response_status[] =
{
    std::make_pair<int, std::string>(100, "Continue"),
    std::make_pair<int, std::string>(101, "Switching Protocols"),
    std::make_pair<int, std::string>(102, "Processing"),
    std::make_pair<int, std::string>(103, "Early Hints"),
    std::make_pair<int, std::string>(200, "OK"),
    std::make_pair<int, std::string>(201, "Created"),
    std::make_pair<int, std::string>(202, "Accepted"),
    std::make_pair<int, std::string>(203, "Non-Authoritative Information"),
    std::make_pair<int, std::string>(204, "No Content"),
    std::make_pair<int, std::string>(205, "Reset Content"),
    std::make_pair<int, std::string>(206, "Partial Content"),
    std::make_pair<int, std::string>(207, "Multi-Status"),
    std::make_pair<int, std::string>(208, "Already Reported"),
    std::make_pair<int, std::string>(226, "IM Used"),
    std::make_pair<int, std::string>(300, "Multiple Choices"),
    std::make_pair<int, std::string>(301, "Moved Permanently"),
    std::make_pair<int, std::string>(302, "Found"),
    std::make_pair<int, std::string>(303, "See Other"),
    std::make_pair<int, std::string>(304, "Not Modified"),
    std::make_pair<int, std::string>(305, "Use Proxy"),
    std::make_pair<int, std::string>(306, "Switch Proxy"),
    std::make_pair<int, std::string>(307, "Temporary Redirect"),
    std::make_pair<int, std::string>(308, "Permanent Redirect"),
    std::make_pair<int, std::string>(400, "Bad Request"),
    std::make_pair<int, std::string>(401, "Unauthorized"),
    std::make_pair<int, std::string>(402, "Payment Required"),
    std::make_pair<int, std::string>(403, "Forbidden"),
    std::make_pair<int, std::string>(404, "Not Found"),
    std::make_pair<int, std::string>(405, "Method Not Allowed"),
    std::make_pair<int, std::string>(406, "Not Acceptable"),
    std::make_pair<int, std::string>(407, "Proxy Authentication Required"),
    std::make_pair<int, std::string>(408, "Request Timeout"),
    std::make_pair<int, std::string>(409, "Conflict"),
    std::make_pair<int, std::string>(410, "Gone"),
    std::make_pair<int, std::string>(411, "Length Required"),
    std::make_pair<int, std::string>(412, "Precondition Failed"),
    std::make_pair<int, std::string>(413, "Payload Too Large"),
    std::make_pair<int, std::string>(414, "URI Too Long"),
    std::make_pair<int, std::string>(415, "Unsupported Media Type"),
    std::make_pair<int, std::string>(416, "Range Not Satisfiable"),
    std::make_pair<int, std::string>(417, "Expectation Failed"),
    std::make_pair<int, std::string>(418, "I'm a teapot"),
    std::make_pair<int, std::string>(421, "Misdirected Request"),
    std::make_pair<int, std::string>(422, "Unprocessable Entity"),
    std::make_pair<int, std::string>(423, "Locked"),
    std::make_pair<int, std::string>(424, "Failed Dependency"),
    std::make_pair<int, std::string>(425, "Too Early"),
    std::make_pair<int, std::string>(426, "Upgrade Required"),
    std::make_pair<int, std::string>(428, "Precondition Required"),
    std::make_pair<int, std::string>(429, "Too Many Requests"),
    std::make_pair<int, std::string>(431, "Request Header Fields Too Large"),
    std::make_pair<int, std::string>(451, "Unavailable For Legal Reasons"),
    std::make_pair<int, std::string>(500, "Internal Server Error"),
    std::make_pair<int, std::string>(501, "Not Implemented"),
    std::make_pair<int, std::string>(502, "Bad Gateway"),
    std::make_pair<int, std::string>(503, "Service Unavailable"),
    std::make_pair<int, std::string>(504, "Gateway Timeout"),
    std::make_pair<int, std::string>(505, "HTTP Version Not Supported"),
    std::make_pair<int, std::string>(506, "Variant Also Negotiates"),
    std::make_pair<int, std::string>(507, "Insufficient Storage"),
    std::make_pair<int, std::string>(508, "Loop Detected"),
    std::make_pair<int, std::string>(510, "Not Extended"),
    std::make_pair<int, std::string>(511, "Network Authentication Required")
};

std::string Respond::get_response_status(int status_code)
{
    size_t status_size = sizeof(response_status) / sizeof(response_status[0]);
    for (size_t i = 0; i < status_size; i++)
    {
        if (response_status[i].first == status_code)
            return (response_status[i].second);
    }
    return (response_status[status_size - 1].second);
}