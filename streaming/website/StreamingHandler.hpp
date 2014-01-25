#ifndef STREAMING_HANDLER_HPP
#define STREAMING_HANDLER_HPP

#include <string>
#include <vector>

namespace network{ class HttpHandler; }

namespace website
{
	class StreamingInfo;

    class StreamingHandler
    {
        public:
            StreamingHandler();
            virtual ~StreamingHandler();

            virtual std::vector<StreamingInfo> getStreamingLinks(const std::string& teamName) = 0;
    };
}

#endif
