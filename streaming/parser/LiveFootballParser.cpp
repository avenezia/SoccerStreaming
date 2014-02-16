#include "LiveFootballParser.hpp"

#include <boost/regex.hpp>
#include <cstring>
#include <string>
using namespace std;

#include "gumbo.h"

namespace parser
{
    // The regular expression used to get the id of a match in a link
    const boost::regex LiveFootballParser::kMatchIdRegExp("(/)([0-9]+)(-)");
    // The value of the class attribute of the span element, parent of the links
    const string LiveFootballParser::kSpanClassValue("argr_custom");

    LiveFootballParser::LiveFootballParser():
        parseTree_(nullptr, gumboPtrDeleter),
        teamName_("")
    {

    }

    LiveFootballParser::~LiveFootballParser()
    {

    }

    string LiveFootballParser::getLinkForTeamMatch(const string& indexPage,
            const string& teamName)
    {
        teamName_ = teamName;
        parsePage(indexPage);
        return searchLinkForTeamMatch(parseTree_->root);
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

    void LiveFootballParser::parseMatchId(const std::string& linkToMatchPage)
    {
        boost::smatch regExpMatch;
        if (boost::regex_search(linkToMatchPage, regExpMatch, kMatchIdRegExp))
        {
            matchId_ = regExpMatch[2].str();
            cout << matchId_ << endl;
        }
        else
        {
            cerr << "LiveFootballParser - Unable to parse the match id for " << linkToMatchPage << endl;
        }
    }

    void LiveFootballParser::parsePage(const string& indexPage)
    {
        parseTree_.reset(gumbo_parse(indexPage.c_str()));
    }

    void LiveFootballParser::parseTableWithStreamingLinks(GumboNode* parentNode)
    {
        if (parentNode != nullptr)
        {
            GumboVector* childrenList = &parentNode->v.element.children;
            GumboNode* currentChild = nullptr;
            int tableCounter = 0;
            for (unsigned int i = 0; i < childrenList->length && tableCounter < 2; ++i)
            {
                currentChild = static_cast<GumboNode*>(childrenList->data[i]);
                if (currentChild != nullptr &&
                    currentChild->type == GUMBO_NODE_ELEMENT &&
                    currentChild->v.element.tag == GUMBO_TAG_TABLE)
                {
                    ++tableCounter;
                }
            }

            // TODO: continue parsing
        }
    }

    string LiveFootballParser::searchLinkForTeamMatch(const GumboNode *node)
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
                string linkOfMatch(href->value);
                // Storing the id of the match in order to use it when parsing
                // the page with the information on the streaming of the match
                parseMatchId(linkOfMatch);
                return linkOfMatch;
            }
        }

        const GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            string currentResult = searchLinkForTeamMatch(static_cast<const GumboNode*>(children->data[i]));
            if (currentResult != "")
            {
                return currentResult;
            }
        }

        return "";
    }

    GumboNode* searchParentDivForMatch(GumboNode* node, const char* matchId)
    {
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return nullptr;
        }

        if (node->v.element.tag == GUMBO_TAG_DIV)
        {
            GumboAttribute* id = gumbo_get_attribute(&node->v.element.attributes, "id");
            if (id != nullptr && strstr(id->value, matchId) != nullptr)
            {
                return node;
            }
        }

        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            GumboNode* currentResult = searchParentDivForMatch(static_cast<GumboNode*>(children->data[i]), matchId);
            if (currentResult != nullptr)
                return currentResult;
        }

        return nullptr;
    }

    void gumboPtrDeleter(GumboOutput *gumboPtr)
    {
        if (gumboPtr != nullptr)
        {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboPtr);
        }
    }
}
