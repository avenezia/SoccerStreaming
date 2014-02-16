#ifndef LIVE_FOOTBALL_PARSER_HPP_
#define LIVE_FOOTBALL_PARSER_HPP_

#include <boost/regex.hpp>
#include <memory>
#include <string>

#include "gumbo.h"

namespace parser
{
    class LiveFootballParser
    {
        public:
            LiveFootballParser();
            ~LiveFootballParser();

            std::string getLinkForTeamMatch(const std::string& indexPage,
                    const std::string& teamName);
        private:
            bool isParentOfMatchLink(const GumboNode *node);
            void parseMatchId(const std::string& linkToMatchPage);
            void parsePage(const std::string& indexPage);
            void parseTableWithStreamingLinks(GumboNode* parentNode);
            std::string searchLinkForTeamMatch(const GumboNode *node);
            GumboNode* searchParentDivForMatch(GumboNode* node, const char* matchId);

            static const boost::regex kMatchIdRegExp;
            static const std::string kSpanClassValue;

            std::string matchId_;
            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;
            std::string teamName_;
    };

    void gumboPtrDeleter(GumboOutput *gumboPtr);
}

#endif /* LIVEFOOTBALLPARSE_HPP_ */
