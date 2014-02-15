#include "LiveFootballParser.hpp"

#include <cstring>
#include <string>

#include "gumbo.h"

namespace parser
{
    // The value of the class attribute of the span element, parent of the links
    const std::string LiveFootballParser::kSpanClassValue("argr_custom");

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
        teamName_ = teamName;
        parsePage(indexPage);
        return searchLinkForTeam(parseTree_->root);
    }

    // Utility method to check if the node can be the parent of the match links
    bool LiveFootballParser::isParentOfMatchLink(const GumboNode *node)
    {
        if (node == nullptr)
            return false;
        if (node->v.element.tag == GUMBO_TAG_SPAN)
        {
            GumboAttribute* classAttribute;
            if ((classAttribute = gumbo_get_attribute(&node->v.element.attributes, "class")) &&
                strstr(classAttribute->value, kSpanClassValue.c_str()) != nullptr)
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

    std::string LiveFootballParser::searchLinkForTeam(const GumboNode *node)
    {
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return "";
        }

        // The links we are interested in are the ones that are children of
        // span elements with class equal to the value in kSpanClassValue variable
        // (Currently argr_custom). Following jquery notation, it is $("span.argr_custom a")
        GumboAttribute* href;
        if (node->v.element.tag == GUMBO_TAG_A &&
            (href = gumbo_get_attribute(&node->v.element.attributes, "href")) &&
            isParentOfMatchLink(node->parent))
        {
            // Checking if the link contains the name of the team
            if (strstr(href->value, teamName_.c_str()) != nullptr)
            {
                return href->value;
            }
        }

        const GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            std::string currentResult = searchLinkForTeam(static_cast<const GumboNode*>(children->data[i]));
            if (currentResult != "")
            {
                return currentResult;
            }
        }

        return "";
    }

    void gumboPtrDeleter(GumboOutput *gumboPtr)
    {
        if (gumboPtr != nullptr)
        {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboPtr);
        }
    }
}
