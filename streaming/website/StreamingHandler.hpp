#ifndef STREAMING_HANDLER_HPP
#define STREAMING_HANDLER_HPP

#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace network{ class HttpHandler; }

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
