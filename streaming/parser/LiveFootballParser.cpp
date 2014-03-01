#include "LiveFootballParser.hpp"

#include <boost/regex.hpp>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

#include "gumbo.h"
#include "StreamingInfo.hpp"

namespace parser
{
    // The regular expression used to get the id of a match in a link
    const boost::regex LiveFootballParser::kMatchIdRegExp("(/)([0-9]+)(-)");
    // The value of the class attribute of the span element, parent of the links
    const string LiveFootballParser::kSpanClassValue("argr_custom");
    // The index of the table containing the links of the streaming for a match
    const int LiveFootballParser::kLinkTableIndex = 2;

    LiveFootballParser::LiveFootballParser():
        matchId_(""),
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

    vector<website::StreamingInfo> LiveFootballParser::getStreamingLinks(const string& matchPage)
    {
        parsePage(matchPage);
        const GumboNode* divNode = searchParentDivForMatch(parseTree_->root);
        if (divNode != nullptr)
        {
            const GumboVector* tableRowList = getNodesWithStreamingLinks(divNode);
            if (tableRowList != nullptr)
            {

            }
            else
            {
                //TODO: remove me
                cerr << "Error while searching tbody" << endl;
            }
        }
        else
        {
            //TODO: remove me
            cerr << "Error while searching div" << endl;
        }

        vector<website::StreamingInfo> result;
        return result;
    }

    /// The method returns the list of <tr> elements containing the information for a specific streaming
    const GumboVector* LiveFootballParser::getNodesWithStreamingLinks(const GumboNode* divParentNode) const
    {
        if (divParentNode == nullptr)
        {
            cerr << "LiveFootballParser - div Node is null" << endl;
            return nullptr;
        }

        const GumboVector* divChildrenList = &divParentNode->v.element.children;
        const GumboNode* divChild = nullptr;
        int tableCounter = 0;
        // Looking for the second table that is the child of the divParentNode
        for (unsigned int i = 0; i < divChildrenList->length && tableCounter < kLinkTableIndex; ++i)
        {
            divChild = static_cast<GumboNode*>(divChildrenList->data[i]);

            if (isNodeOfSpecificTypeAndTag(divChild, GUMBO_TAG_TABLE))
            {
                ++tableCounter;
            }
        }

        if (divChild != nullptr && tableCounter == kLinkTableIndex)
        {
            // divChild is the table containing the <tr> elements with the
            // links to the streaming: <table><tbody><tr></tr><tr></tr></tbody></table>
            const GumboVector* tableChildrenList = &divChild->v.element.children;
            const GumboNode* tableChild = nullptr;
            // Looking for the tbody element in order to return its children,
            // the tr elements containing the link for the streaming
            for (unsigned int i = 0; i < tableChildrenList->length; ++i)
            {
                tableChild = static_cast<GumboNode*>(tableChildrenList->data[i]);
                if (isNodeOfSpecificTypeAndTag(tableChild, GUMBO_TAG_TBODY))
                {
                    return &tableChild->v.element.children;
                }
            }
        }

        return nullptr;
    }

    /// Utility method to check if the node matches a specific type and tag
    bool LiveFootballParser::isNodeOfSpecificTypeAndTag(const GumboNode* node, GumboTag nodeTag,
            GumboNodeType nodeType) const
    {
        if (node != nullptr &&
            node->type == nodeType &&
            node->v.element.tag == nodeTag)
        {
            return true;
        }

        return false;
    }

    /// Utility method to check if the node can be the <span> parent of the <a>
    /// element containing the link for the match
    /// For example
    /// <span class="argr_custom more">
    ///   <a href="http://livefootball.ws/13328-champions-league-galatasaray-chelsea.html"></a>
    /// </span>
    bool LiveFootballParser::isParentOfMatchLink(const GumboNode *node) const
    {
        if (isNodeOfSpecificTypeAndTag(node, GUMBO_TAG_SPAN))
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

    /// The method parses the link to a page containing the streamings of a match
    /// and store the id of the match. For example, in the following link
    /// http://livefootball.ws/13328-champions-league-galatasaray-chelsea.html
    // it will store the id 13328
    void LiveFootballParser::parseMatchId(const std::string& linkToMatchPage)
    {
        boost::smatch regExpMatch;
        if (boost::regex_search(linkToMatchPage, regExpMatch, kMatchIdRegExp))
        {
            matchId_ = regExpMatch[2].str();
        }
        else
        {
            cerr << "LiveFootballParser - Unable to parse the match id for " << linkToMatchPage << endl;
        }
    }

    /// The method parses the list of <tr> elements containing the information for the streaming
    vector<website::StreamingInfo> LiveFootballParser::parseNodesWithStreamingLinks(const GumboVector* trElementList) const
    {
        vector<website::StreamingInfo> streamingLinks;
        if (trElementList != nullptr)
        {
            for (unsigned int i = 0; i < trElementList->length; ++i)
            {
                //GumboNode* trElement = static_cast<GumboNode*>(trElementList->data[i]);
                //streamingLinks.push_back()
            }
        }
        else
        {
            cerr << "LiveFootballParser - The list of <tr> elements containing the streaming info is null" << endl;
        }

        return streamingLinks;
    }

    // The method parses a single <tr> element containing the information for the streaming
    website::StreamingInfo LiveFootballParser::parseNodeWithStreamingLink(const GumboNode* trElement) const
    {
        website::StreamingInfo streamingInfo;
        if (trElement != nullptr)
        {
            const GumboVector* trChildrenList = &trElement->v.element.children;
            const GumboNode* trChild = nullptr;
            // Each child should be a <td> element containing some information
            for (unsigned int tdIndex = 0; tdIndex < trChildrenList->length; ++tdIndex)
            {
                trChild = static_cast<GumboNode*>(trChildrenList->data[tdIndex]);
                if (isNodeOfSpecificTypeAndTag(trChild, GUMBO_TAG_TD))
                {
                }
            }
        }
        else
        {
            cerr << "LiveFootballParser - A <tr> element containing the streaming info is null" << endl;
        }

        return streamingInfo;
    }

    void LiveFootballParser::parsePage(const string& htmlPage)
    {
        parseTree_.reset(gumbo_parse(htmlPage.c_str()));
    }

    /// The method searches the link to the details of the streaming for a specific team
    string LiveFootballParser::searchLinkForTeamMatch(const GumboNode* node)
    {
        if (node == nullptr || node->type != GUMBO_NODE_ELEMENT)
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

        const GumboVector* children = &node->v.element.children;
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

    /// This method looks for the div (addressable by id) inside the web page
    /// containing the details of the streaming for one specific match
    const GumboNode* LiveFootballParser::searchParentDivForMatch(const GumboNode* node) const
    {
        if (node == nullptr || node->type != GUMBO_NODE_ELEMENT)
        {
            return nullptr;
        }

        if (node->v.element.tag == GUMBO_TAG_DIV)
        {
            GumboAttribute* id = gumbo_get_attribute(&node->v.element.attributes, "id");
            if (id != nullptr && strstr(id->value, matchId_.c_str()) != nullptr)
            {
                return node;
            }
        }

        const GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            const GumboNode* currentResult = searchParentDivForMatch(static_cast<GumboNode*>(children->data[i]));
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
