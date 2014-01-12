#ifndef CURL_HTTP_HANDLER_HPP
#define CURL_HTTP_HANDLER_HPP

#include "HttpHandler.hpp"

#include <string>
#include <vector>

#include <curlpp/Easy.hpp>

namespace network
{
    class CurlHttpHandler : public HttpHandler
    {
        public:
            explicit CurlHttpHandler(const std::string& hostnameUrl);
            virtual ~CurlHttpHandler();

            virtual short handleGet(const std::string& requestUri, 
                const std::vector<std::pair<std::string, std::string>>& headerList,
                std::string& httpResponseBody);     
            virtual short handleGet(const std::string& requestUri, 
                std::string& httpResponseBody);

        private:
            std::string hostnameUrl_;
            curlpp::Easy requestHandler_;
    };
}

#endif