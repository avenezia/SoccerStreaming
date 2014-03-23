#ifndef PARSER_UTILS_HPP_
#define PARSER_UTILS_HPP_

#include "gumbo.h"
#include <string>

namespace parser
{
    class ParserUtils
    {
        public:
            static const GumboNode* getElementById(const GumboNode* node,
                    GumboTag elementTag,
                    const std::string& idValue);
            static bool isNodeOfTypeAndTag(const GumboNode* node,
                    GumboTag nodeTag,
                    GumboNodeType nodeType = GUMBO_NODE_ELEMENT);
    };
}
#endif /* PARSER_UTILS_HPP_ */
