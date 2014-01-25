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
                const std::vector<std::pair<std::string, std::string>>& headerList) = 0;
            virtual HttpResponse getRequest(const std::string& requestUri) = 0;
    };
}

#endif
