#ifndef LIVE_FOOTBALL_PARSER_HPP_
#define LIVE_FOOTBALL_PARSER_HPP_

#include <boost/regex.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include "gumbo.h"

#include "HtmlParser.hpp"

namespace website
{
    class StreamingInfo;
}

namespace parser
{
    class LiveFootballParser : public HtmlParser
    {
        public:
            LiveFootballParser() = default;
            ~LiveFootballParser() = default;

            std::string getLinkForTeamMatch(const std::string& indexPage,
                    const std::string& teamName);
            std::vector<website::StreamingInfo> getStreamingLinks(const std::string& matchPage);

        private:
            enum class FieldIndex : std::uint32_t
            {
                BITRATE = 3,
                CHANNEL = 4,
                LINK = 7
            };

            static const std::string kContainerDivIdPrefix;
            static const boost::regex kMatchIdRegExp;
            static const std::string kSpanClassValue;
            static const int kLinkTableIndex;

            std::vector<const GumboNode*> getTrListWithStreamingLinks(const GumboNode* divParentNode) const;

            bool isParentOfMatchLink(const GumboNode *node) const;

            void parseMatchId(const std::string& linkToMatchPage);
            std::string parseTdContainingLink(const GumboNode* tdElement) const;
            website::StreamingInfo parseTrElementWithStreamingLink(const GumboNode* trElement) const;
            std::vector<website::StreamingInfo> parseTrListWithStreamingLinks(const std::vector<const GumboNode*>& trElementList) const;
            std::string searchLinkForTeamMatch(const GumboNode *node);

            std::string matchId_;
            std::string teamName_;
    };
}

#endif /* LIVEFOOTBALLPARSER_HPP_ */
