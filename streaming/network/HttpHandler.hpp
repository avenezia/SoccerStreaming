#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include <string>
#include <vector>

namespace network
{
    class HttpHandler
    {
        public:
            HttpHandler() {};
            explicit HttpHandler(const std::string& hostnameUrl) {};
            virtual ~HttpHandler() {};
            
            virtual short getRequest(const std::string& requestUri,
                const std::vector<std::pair<std::string, std::string>>& headerList,
                std::string& httpResponseBody) = 0;
            virtual short getRequest(const std::string& requestUri,
                std::string& httpResponseBody) = 0;
    };
}

#endif