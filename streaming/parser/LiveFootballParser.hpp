#ifndef LIVE_FOOTBALL_PARSER_HPP_
#define LIVE_FOOTBALL_PARSER_HPP_

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
            void parsePage(const std::string& indexPage);
            std::string searchLinkForTeam(const GumboNode *node);

            static const std::string kSpanClassValue;

            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;
            std::string teamName_;
    };

    void gumboPtrDeleter(GumboOutput *gumboPtr);
}

#endif /* LIVEFOOTBALLPARSE_HPP_ */
