#include "ParserUtils.hpp"

#include <cstring>
#include <string>

namespace parser
{
    const GumboNode* ParserUtils::getElementById(const GumboNode* node,
            GumboTag elementTag,
            const std::string& idValue)
    {
        if (node == nullptr || node->type != GUMBO_NODE_ELEMENT)
        {
            return nullptr;
        }

        if (node->v.element.tag == elementTag)
        {
            const GumboAttribute* id = gumbo_get_attribute(&node->v.element.attributes, "id");
            if (id != nullptr && strstr(id->value, idValue.c_str()) != nullptr)
            {
                return node;
            }
        }

        const GumboVector* childrenList = &node->v.element.children;
        for (unsigned int childIndex = 0; childIndex < childrenList->length; ++childIndex)
        {
            const GumboNode* child = static_cast<GumboNode*>(childrenList->data[childIndex]);
            const GumboNode* currentResult = getElementById(child, elementTag, idValue);
            if (currentResult != nullptr)
                return currentResult;
        }

        return nullptr;
    }

    bool ParserUtils::isNodeOfTypeAndTag(const GumboNode* node,
            GumboTag nodeTag,
            GumboNodeType nodeType)
    {
        if (node != nullptr &&
            node->type == nodeType &&
            node->v.element.tag == nodeTag)
        {
            return true;
        }

        return false;
    }
}
