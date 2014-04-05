#ifndef PARSER_UTILS_HPP_
#define PARSER_UTILS_HPP_

#include "gumbo.h"
#include <cassert>
#include <string>
#include <vector>

namespace parser
{
    class ParserUtils
    {
        public:
            static std::string getAttribute(const GumboNode* node, const std::string& attributeName);

            // In the header only to avoid issues with templates
            template <typename Predicate>
            static const GumboNode* getChildWithPredicate(const GumboNode* parent, GumboTag elementTag, Predicate p)
            {
                assert(parent != nullptr);
                const GumboVector* childrenList = &parent->v.element.children;
                for (unsigned int childIndex = 0; childIndex < childrenList->length; ++childIndex)
                {
                    const GumboNode* child = static_cast<GumboNode*>(childrenList->data[childIndex]);
                    if (ParserUtils::isNodeOfTypeAndTag(child, elementTag) && p(child))
                    {
                        return child;
                    }
                }

                return nullptr;
            }

            static std::vector<const GumboNode*> getChildrenOfTag(const GumboNode* parent, GumboTag elementTag);
            static const GumboNode* getElementById(const GumboNode* node,
                    GumboTag elementTag,
                    const std::string& idValue);
            static const GumboNode* getIthChildOfTag(const GumboNode* parent, int index, GumboTag elementTag);
            static std::string getTextForElement(const GumboNode* node);
            static bool hasAttribute(const GumboNode* node, const std::string& attributeName);
            static bool isNodeOfTypeAndTag(const GumboNode* node,
                    GumboTag nodeTag,
                    GumboNodeType nodeType = GUMBO_NODE_ELEMENT);
    };

    void gumboPtrDeleter(GumboOutput* gumboPtr);
}
#endif /* PARSER_UTILS_HPP_ */
