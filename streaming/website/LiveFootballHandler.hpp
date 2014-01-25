#ifndef LIVE_FOOTBALL_HANDLER_HPP
#define LIVE_FOOTBALL_HANDLER_HPP

#include "StreamingHandler.hpp"
#include "StreamingInfo.hpp"

#include <memory>

namespace network{ class HttpHandler; }

namespace website
{
	class StreamingInfo;

    class LiveFootballHandler : public StreamingHandler
    {
        public:
            LiveFootballHandler(const std::string& hostName);
            virtual ~LiveFootballHandler();

            virtual std::vector<StreamingInfo> getStreamingLinks(const std::string& teamName);

        private:
            std::unique_ptr<network::HttpHandler> httpHandler_;
    };
}

#endif
