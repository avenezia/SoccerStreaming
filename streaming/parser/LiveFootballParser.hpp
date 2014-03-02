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
            const GumboVector* getTrListWithStreamingLinks(const GumboNode* divParentNode) const;
            std::string getTextForElement(const GumboNode* node) const;
            bool isNodeOfSpecificTypeAndTag(const GumboNode* node, GumboTag nodeTag,
                    GumboNodeType nodeType = GUMBO_NODE_ELEMENT) const;
            bool isParentOfMatchLink(const GumboNode *node) const;
            void parseMatchId(const std::string& linkToMatchPage);
            website::StreamingInfo parseTrElementWithStreamingLink(const GumboNode* trElement) const;
            std::vector<website::StreamingInfo> parseTrListWithStreamingLinks(const GumboVector* trElementList) const;
            void parsePage(const std::string& htmlPage);
            std::string searchLinkForTeamMatch(const GumboNode *node);
            const GumboNode* searchParentDivForMatch(const GumboNode* node) const;

            static const boost::regex kMatchIdRegExp;
            static const std::string kSpanClassValue;
            static const int kLinkTableIndex;

            std::string matchId_;
            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;
            std::string teamName_;

            enum class FieldIndex : std::uint32_t
            {
                BITRATE = 3,
                CHANNEL = 4,
                LINK = 7
            };
    };

    void gumboPtrDeleter(GumboOutput *gumboPtr);
}

#endif /* LIVEFOOTBALLPARSE_HPP_ */
