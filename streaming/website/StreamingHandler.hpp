#ifndef STREAMING_HANDLER_HPP
#define STREAMING_HANDLER_HPP

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
}

#endif
