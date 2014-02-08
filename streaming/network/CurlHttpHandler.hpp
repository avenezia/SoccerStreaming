#ifndef CURL_HTTP_HANDLER_HPP
#define CURL_HTTP_HANDLER_HPP

#include "HttpHandler.hpp"

#include <string>
#include <vector>

#include <curlpp/Easy.hpp>

namespace network
{
    class HttpResponse;

    class CurlHttpHandler : public HttpHandler
    {
        public:
            explicit CurlHttpHandler(const std::string& hostnameUrl);
            virtual ~CurlHttpHandler();

            virtual HttpResponse getRequest(const std::string& requestUri,
                const std::vector<std::pair<std::string, std::string>>& headers);
            virtual HttpResponse getRequest(const std::string& requestUri,
                const std::pair<std::string, std::string>& singleHeader);
            virtual HttpResponse getRequest(const std::string& requestUri);

        private:
            std::string hostnameUrl_;
            curlpp::Easy requestHandler_;
    };
}

#endif
