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
    };
}
#endif /* PARSER_UTILS_HPP_ */
