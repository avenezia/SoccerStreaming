#ifndef HTML_PARSER_HPP_
#define HTML_PARSER_HPP_

#include "gumbo.h"

#include <memory>
#include <string>

namespace parser
{
    class HtmlParser
    {
        public:
            HtmlParser();
            ~HtmlParser();
            //TODO: it could be better to have getStreamingLinks in this base class
        protected:
            void parsePage(const std::string& htmlPage);

            std::unique_ptr<GumboOutput, void(*)(GumboOutput*)> parseTree_;

        private:

    };
}
#endif
