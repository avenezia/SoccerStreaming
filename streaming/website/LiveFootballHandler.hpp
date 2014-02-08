#ifndef LIVE_FOOTBALL_HANDLER_HPP
#define LIVE_FOOTBALL_HANDLER_HPP

#include "StreamingHandler.hpp"
#include "StreamingInfo.hpp"

#include <memory>

#include <boost/regex.hpp>

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
            static const boost::regex kAccessCookieRegExp;

            std::string getAccessCookieFromHomePage() const;
            std::string getRealHomePage(const std::string& accessCookie) const;
            std::string parseAccessCookie(const std::string& webPage) const;

            std::unique_ptr<network::HttpHandler> httpHandler_;
    };
}

#endif
