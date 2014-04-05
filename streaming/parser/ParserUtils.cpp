#include "ParserUtils.hpp"

#include <cassert>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

namespace parser
{
    string ParserUtils::getAttribute(const GumboNode* node, const string& attributeName)
    {
        assert(node != nullptr);
        const GumboAttribute* nodeAttribute = gumbo_get_attribute(&node->v.element.attributes, attributeName.c_str());
        if (nodeAttribute != nullptr)
        {
            return nodeAttribute->value;
        }

        return "";
    }

    vector<const GumboNode*> ParserUtils::getChildrenOfTag(const GumboNode* parent, GumboTag elementTag)
    {
        assert(parent != nullptr);
        vector<const GumboNode*> childrenVector;
        const GumboVector* childrenList = &parent->v.element.children;
        for (unsigned int childIndex = 0; childIndex < childrenList->length; ++childIndex)
        {
            const GumboNode* child = static_cast<GumboNode*>(childrenList->data[childIndex]);
            if (ParserUtils::isNodeOfTypeAndTag(child, elementTag))
            {
                childrenVector.push_back(child);
            }
        }

        return childrenVector;
    }

    const GumboNode* ParserUtils::getElementById(const GumboNode* node,
            GumboTag elementTag,
            const string& idValue)
    {
        if (node == nullptr || node->type != GUMBO_NODE_ELEMENT)
        {
            return nullptr;
        }

        if (node->v.element.tag == elementTag)
        {
            if (getAttribute(node, "id") == idValue)
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

    const GumboNode* ParserUtils::getIthChildOfTag(const GumboNode* parent, int index, GumboTag elementTag)
    {
        assert(parent != nullptr);
        const GumboVector* childrenList = &parent->v.element.children;
        // This counter keeps track only of the children for the requested tag
        int childIndexOfTag = 0;
        for (unsigned int childIndex = 0; childIndex < childrenList->length; ++childIndex)
        {
            const GumboNode* child = static_cast<GumboNode*>(childrenList->data[childIndex]);
            if (ParserUtils::isNodeOfTypeAndTag(child, elementTag))
            {
                if (childIndexOfTag == index)
                {
                    return child;
                }
                ++childIndexOfTag;
            }
        }

        return nullptr;
    }

    /// The method returns the text child of the element
    string ParserUtils::getTextForElement(const GumboNode* node)
    {
        if (node != nullptr)
        {
            const GumboVector* childrenList = &node->v.element.children;
            const GumboNode* child = nullptr;
            for (unsigned int childIndex = 0; childIndex < childrenList->length; ++childIndex)
            {
                child = (GumboNode*) childrenList->data[childIndex];
                if (child->type == GUMBO_NODE_TEXT)
                {
                    return child->v.text.text;
                }
            }
        }

        return "";
    }

    bool ParserUtils::hasAttribute(const GumboNode* node, const string& attributeName)
    {
        assert(node != nullptr);
        return gumbo_get_attribute(&node->v.element.attributes, attributeName.c_str()) != nullptr;
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

    void gumboPtrDeleter(GumboOutput* gumboPtr)
    {
        if (gumboPtr != nullptr)
        {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboPtr);
        }
    }
}
