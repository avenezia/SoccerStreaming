#ifndef LIVE_FOOTBALL_HANDLER_HPP
#define LIVE_FOOTBALL_HANDLER_HPP

#include <boost/regex.hpp>
#include <memory>

#include "LiveFootballParser.hpp"
#include "StreamingHandler.hpp"

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

            std::string getMatchPage(const std::string& matchPageLink) const;
            std::string getRealHomePage() const;
            std::string parseAccessCookie(const std::string& webPage) const;
            void setAccessCookieFromHomePage();

            // It seems that a common interface for different parsers is not easy
            // to design: maybe in the future it can be improved
            std::string accessCookie_;
            parser::LiveFootballParser htmlParser_;
    };
}

#endif
