#include "LiveFootballParser.hpp"

#include <cstring>
#include <string>

#include "gumbo.h"

namespace parser
{
    LiveFootballParser::LiveFootballParser():
        parseTree_(nullptr, gumboPtrDeleter),
        teamName_("")
    {

    }

    LiveFootballParser::~LiveFootballParser()
    {

    }

    std::string LiveFootballParser::getLinkForTeamMatch(const std::string& indexPage,
            const std::string& teamName)
    {
        return "";
    }

    bool LiveFootballParser::isParentOfMatchLink(GumboNode* node)
    {
        if (node == nullptr)
            return false;
        if (node->v.element.tag == GUMBO_TAG_SPAN)
        {
            GumboAttribute* classAttribute;
            if ((classAttribute = gumbo_get_attribute(&node->v.element.attributes, "class")) &&
                strstr(classAttribute->value, "argr_custom") != nullptr)
            {
                return true;
            }
        }

        return false;
    }

    void LiveFootballParser::parsePage(const std::string& indexPage)
    {
        parseTree_.reset(gumbo_parse(indexPage.c_str()));
    }

    std::string LiveFootballParser::searchLinkForTeam(GumboNode* node)
    {
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return "";
        }

        GumboAttribute* href;
        if (node->v.element.tag == GUMBO_TAG_A &&
            (href = gumbo_get_attribute(&node->v.element.attributes, "href")) &&
            isParentOfMatchLink(node->parent))
        {
            if (strstr(href->value, teamName_.c_str()) != nullptr)
            {
                return href->value;
            }
        }

        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            std::string currentResult = searchLinkForTeam(static_cast<GumboNode*>(children->data[i]));
            if (currentResult != "")
            {
                return currentResult;
            }
        }

        return "";
    }

    void gumboPtrDeleter(GumboOutput* gumboPtr)
    {
        if (gumboPtr != nullptr)
        {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboPtr);
        }
    }
}
