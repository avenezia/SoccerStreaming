#ifndef STREAMING_HANDLER_HPP
#define STREAMING_HANDLER_HPP

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "HttpHandler.hpp"


namespace website
{
	class StreamingInfo;

    class StreamingHandler
    {
        public:
	        enum class StreamingHandlerType
	        {
	            LIVEFOOTBALL,
	            ROJADIRECTA
	        };

            StreamingHandler() = default;
            virtual ~StreamingHandler() = default;

            virtual std::vector<StreamingInfo> getStreamingLinks(const std::string& teamName) = 0;
            static std::unique_ptr<StreamingHandler> makeStreamingHandler(StreamingHandlerType type);

        protected:
            std::string performHttpRequest(const std::string& pageUrl,
                    bool withAbsolutePath) const;
            std::string performHttpRequest(const std::string& pageUrl,
                    bool withAbsolutePath,
                    const std::pair<std::string, std::string>& header) const;

            std::unique_ptr<network::HttpHandler> httpHandler_;
    };

    class UnknownStreamingHandler : public std::exception
    {
        const char* what() const noexcept override
        {
            return "Unknown Handler for streaming";
        }
    };
}

#endif
