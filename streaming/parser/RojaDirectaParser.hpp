#ifndef ROJA_DIRECTA_PARSER_HPP_
#define ROJA_DIRECTA_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "gumbo.h"

namespace website
{
    class StreamingInfo;
}

namespace parser
{
    class RojaDirectaParser
    {
        public:
            RojaDirectaParser();
            ~RojaDirectaParser();

            std::vector<website::StreamingInfo> getStreamingLinks(const std::string& matchPage);

        private:
            static const std::string kContainerDivId;

            void parsePage(const std::string& htmlPage);

            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;
            std::string teamName_;
    };
}

#endif /* ROJA_DIRECTA_PARSER_HPP_ */
