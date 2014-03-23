#include "LiveFootballParser.hpp"

#include <boost/regex.hpp>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#include "gumbo.h"
#include "ParserUtils.hpp"
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

    /// The method returns the list of StreamingInfo object containing the information parse from the web page
    vector<website::StreamingInfo> LiveFootballParser::getStreamingLinks(const string& matchPage)
    {
        vector<website::StreamingInfo> streamingInfoContainer;
        parsePage(matchPage);
        // Searching the div that contains the useful information: it has the id equal to the id of the match
        const GumboNode* divNode = ParserUtils::getElementById(parseTree_->root, GUMBO_TAG_DIV, matchId_);
        if (divNode != nullptr)
        {
            // Getting the list of tr elements containing the links
            const GumboVector* trList = getTrListWithStreamingLinks(divNode);
            if (trList != nullptr)
            {
                streamingInfoContainer = parseTrListWithStreamingLinks(trList);
            }
            else
            {
                cerr << "LiveFootballParser - unable to find the tr elements containing links" << endl;
            }
        }
        else
        {
            cerr << "LiveFootballParser - unable to find the container div" << endl;
        }

        return streamingInfoContainer;
    }

    /// The method returns the list of <tr> elements containing the information for a specific streaming
    const GumboVector* LiveFootballParser::getTrListWithStreamingLinks(const GumboNode* divParentNode) const
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

        if (divChild != nullptr &&
            isNodeOfSpecificTypeAndTag(divChild, GUMBO_TAG_TABLE) &&
            tableCounter == kLinkTableIndex)
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
        else
        {
            cerr << "LiveFootballParser - error while searching table with streaming data" << endl;
        }

        return nullptr;
    }

    /// The method returns the text child of the element
    std::string LiveFootballParser::getTextForElement(const GumboNode* node) const
    {
        if (node != nullptr)
        {
            const GumboVector* childrenList = &node->v.element.children;
            const GumboNode* child = nullptr;
            // Each child should be a <td> element containing some information
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
            const GumboAttribute* classAttribute;
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

    string LiveFootballParser::parseTdContainingLink(const GumboNode* tdElement) const
    {
        if (tdElement != nullptr)
        {
            const GumboVector* tdChildrenList = &tdElement->v.element.children;
            const GumboNode* tdChild = nullptr;
            // We are looking for the a element that has the link to the streaming
            // in the href attribute
            for (unsigned int childIndex = 0; childIndex < tdChildrenList->length; ++childIndex)
            {
                tdChild = static_cast<GumboNode*>(tdChildrenList->data[childIndex]);
                if (isNodeOfSpecificTypeAndTag(tdChild, GUMBO_TAG_A))
                {
                    const GumboAttribute* hrefAttribute = gumbo_get_attribute(&tdChild->v.element.attributes, "href");
                    if (hrefAttribute != nullptr)
                    {
                        return hrefAttribute->value;
                    }
                    else
                    {
                        cerr << "LiveFootballParser - The a element containing the link has not href attribute" << endl;
                    }
                }
            }
        }
        else
        {
            cerr << "LiveFootballParser - The <td> element containing the link is null" << endl;
        }

        return "";
    }

    // The method parses a single <tr> element containing the information for the streaming
    website::StreamingInfo LiveFootballParser::parseTrElementWithStreamingLink(const GumboNode* trElement) const
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
                    //TODO: The cast is ugly: check if a better solution can be found
                    if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::BITRATE))
                    {
                        streamingInfo.setBitRate(getTextForElement(trChild));
                    }
                    else if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::CHANNEL))
                    {
                        // TODO: need the conversion from Windows-1251 to utf8
                        streamingInfo.setChannel(getTextForElement(trChild));
                    }
                    else if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::LINK))
                    {
                        streamingInfo.setLink(parseTdContainingLink(trChild));
                    }
                }
            }
        }
        else
        {
            cerr << "LiveFootballParser - A <tr> element containing the streaming info is null" << endl;
        }

        return streamingInfo;
    }

    /// The method parses the list of <tr> elements containing the information for the streaming
    /// and this information is grouped in StreamingInfo objects
    vector<website::StreamingInfo> LiveFootballParser::parseTrListWithStreamingLinks(const GumboVector* trElementList) const
    {
        vector<website::StreamingInfo> streamingLinks;
        if (trElementList != nullptr)
        {
            // The number of tr elements encountered, useful to skip the first one that
            // doesn't contain information but is the header of the table (why they don't use th?)
            int trCount = 0;
            for (unsigned int trIndex = 0; trIndex < trElementList->length; ++trIndex)
            {
                GumboNode* trElement = static_cast<GumboNode*>(trElementList->data[trIndex]);
                // We have to perform the check on tr element because there could be other
                // spurious elements in the children list
                if (isNodeOfSpecificTypeAndTag(trElement, GUMBO_TAG_TR))
                {
                    if (trCount > 0)
                    {
                        // Starting to parse only from the second tr because the first one is the
                        // header of the table! The site should use th instead of this ugly solution
                        // Adding a StreamingInfo object for each row; using move semantics
                        streamingLinks.push_back(move(parseTrElementWithStreamingLink(trElement)));
                    }
                    ++trCount;
                }
            }
        }
        else
        {
            cerr << "LiveFootballParser - The list of <tr> elements containing the streaming info is null" << endl;
        }

        return streamingLinks;
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
        const GumboAttribute* href;
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

    void gumboPtrDeleter(GumboOutput *gumboPtr)
    {
        if (gumboPtr != nullptr)
        {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboPtr);
        }
    }
}
