#ifndef CURL_HTTP_HANDLER_HPP
#define CURL_HTTP_HANDLER_HPP

#include "HttpHandler.hpp"

#include <string>
#include <utility>
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
                const std::vector<std::pair<std::string, std::string>>& headers,
                bool withAbsolutePath = false);
            virtual HttpResponse getRequest(const std::string& requestUri,
                const std::pair<std::string, std::string>& singleHeader,
                bool withAbsolutePath = false);
            virtual HttpResponse getRequest(const std::string& requestUri,
                bool withAbsolutePath = false);

        private:
            static const std::string kHttpBodyStart;

            std::pair<std::string, std::string> getHeaderAndBodyFromStream(const std::string& streamString) const;

            std::string hostnameUrl_;
            curlpp::Easy requestHandler_;
    };
}

#endif
