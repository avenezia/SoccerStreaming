#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include <string>
#include <vector>

namespace network
{
    class HttpResponse;

    class HttpHandler
    {
        public:
            HttpHandler() {};
            explicit HttpHandler(const std::string& hostnameUrl) {};
            virtual ~HttpHandler() {};
            
            virtual HttpResponse getRequest(const std::string& requestUri,
                const std::vector<std::pair<std::string, std::string>>& headerList,
                bool withAbsolutePath = false) = 0;
            virtual HttpResponse getRequest(const std::string& requestUri,
                const std::pair<std::string, std::string>& singleHeader,
                bool withAbsolutePath = false) = 0;
            virtual HttpResponse getRequest(const std::string& requestUri,
                bool withAbsolutePath = false) = 0;
    };
}

#endif
