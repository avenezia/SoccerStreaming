#ifndef PARSER_UTILS_HPP_
#define PARSER_UTILS_HPP_

#include "gumbo.h"
#include <string>

namespace parser
{
    class ParserUtils
    {
        public:
            static std::string getAttribute(const GumboNode* node, const std::string& attributeName);
            static const GumboNode* getElementById(const GumboNode* node,
                    GumboTag elementTag,
                    const std::string& idValue);
            static std::string getTextForElement(const GumboNode* node);
            static bool hasAttribute(const GumboNode* node, const std::string& attributeName);
            static bool isNodeOfTypeAndTag(const GumboNode* node,
                    GumboTag nodeTag,
                    GumboNodeType nodeType = GUMBO_NODE_ELEMENT);
    };

    void gumboPtrDeleter(GumboOutput* gumboPtr);
}
#endif /* PARSER_UTILS_HPP_ */
