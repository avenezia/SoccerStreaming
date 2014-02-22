#ifndef LIVE_FOOTBALL_PARSER_HPP_
#define LIVE_FOOTBALL_PARSER_HPP_

#include <boost/regex.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include "gumbo.h"

namespace website
{
    class StreamingInfo;
}

namespace parser
{
    class LiveFootballParser
    {
        public:
            LiveFootballParser();
            ~LiveFootballParser();

            std::string getLinkForTeamMatch(const std::string& indexPage,
                    const std::string& teamName);
            std::vector<website::StreamingInfo> getStreamingLinks(const std::string& matchPage);

        private:
            bool isParentOfMatchLink(const GumboNode *node);
            void parseMatchId(const std::string& linkToMatchPage);
            void parsePage(const std::string& htmlPage);
            GumboVector* getNodesWithStreamingLinks(GumboNode* divParentNode);
            std::string searchLinkForTeamMatch(const GumboNode *node);
            GumboNode* searchParentDivForMatch(GumboNode* node);

            static const boost::regex kMatchIdRegExp;
            static const std::string kSpanClassValue;
            static const int kLinkTableIndex;

            std::string matchId_;
            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;
            std::string teamName_;

            enum class FieldIndex : std::uint8_t
            {
              BITRATE = 3,
              CHANNEL = 4,
              LINK = 7
            };
    };

    void gumboPtrDeleter(GumboOutput *gumboPtr);
}

#endif /* LIVEFOOTBALLPARSE_HPP_ */
