#ifndef ROJA_DIRECTA_PARSER_HPP_
#define ROJA_DIRECTA_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "gumbo.h"

#include "HtmlParser.hpp"

namespace website
{
    class StreamingInfo;
}

namespace parser
{
    class RojaDirectaParser : public HtmlParser
    {
        public:
            RojaDirectaParser();
            ~RojaDirectaParser();

            std::vector<website::StreamingInfo> getStreamingLinks(const std::string& homePage,
                    const std::string& team);

        private:
            enum class FieldIndex : std::uint32_t
            {
                BITRATE = 4,
                CHANNEL = 1,
                LINK = 5
            };

            static const std::string kContainerDivId;
            static const std::string kLinkPrefix;
            static const std::string kSpanClass;
            static const std::string kTableDivId;

            bool isSpanForTeam(const GumboNode* spanElement) const;
            void parseSpanElement(const GumboNode* spanElement,
                    std::vector<website::StreamingInfo>& resultContainer) const;
            std::string parseTdWithLink(const GumboNode* tdElement) const;
            website::StreamingInfo parseTrElement(const GumboNode* trElement) const;

            std::string teamName_;
    };
}

#endif /* ROJA_DIRECTA_PARSER_HPP_ */
