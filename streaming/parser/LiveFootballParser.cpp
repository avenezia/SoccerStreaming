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
    // The prefix of the value of the id for the div containing links
    const std::string LiveFootballParser::kContainerDivIdPrefix("news-id-");
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
        // Searching the div that contains the useful information: its id is composed by a prefix and the matchId
        const GumboNode* divNode = ParserUtils::getElementById(parseTree_->root, GUMBO_TAG_DIV,
                kContainerDivIdPrefix + matchId_);
        if (divNode != nullptr)
        {
            // Getting the list of tr elements containing the links
            vector<const GumboNode*> trList = getTrListWithStreamingLinks(divNode);
            if (!trList.empty())
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
    std::vector<const GumboNode*> LiveFootballParser::getTrListWithStreamingLinks(const GumboNode* divParentNode) const
    {
        vector<const GumboNode*> trVector;
        if (divParentNode == nullptr)
        {
            cerr << "LiveFootballParser - div Node is null" << endl;
            return trVector;
        }

        // Looking for the second table that is the child of the divParentNode, the index is 0 based
        const GumboNode* secondTableChildOfDiv = ParserUtils::getIthChildOfTag(divParentNode, 1, GUMBO_TAG_TABLE);
        if (secondTableChildOfDiv != nullptr)
        {
            // This is the table containing the <tbody> with the <tr> elements
            // containing the links to the streaming
            const GumboNode* tbodyChildOfTable = ParserUtils::getIthChildOfTag(secondTableChildOfDiv, 0, GUMBO_TAG_TBODY);
            if (tbodyChildOfTable != nullptr)
            {
                // Returning the list of children, the tr elements
                trVector = ParserUtils::getChildrenOfTag(tbodyChildOfTable, GUMBO_TAG_TR);
            }
        }
        else
        {
            cerr << "LiveFootballParser - error while searching table with streaming data" << endl;
        }

        return trVector;
    }

    /// Utility method to check if the node can be the <span> parent of the <a>
    /// element containing the link for the match
    /// For example
    /// <span class="argr_custom more">
    ///   <a href="http://livefootball.ws/13328-champions-league-galatasaray-chelsea.html"></a>
    /// </span>
    bool LiveFootballParser::isParentOfMatchLink(const GumboNode *node) const
    {
        if (ParserUtils::isNodeOfTypeAndTag(node, GUMBO_TAG_SPAN))
        {
            // Using find and not operator== because the class can be composed of several values
            // separated by spaces
            if (ParserUtils::getAttribute(node, "class").find(kSpanClassValue.c_str()) != string::npos)
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

    // The method returns the link inside the <td> element
    string LiveFootballParser::parseTdContainingLink(const GumboNode* tdElement) const
    {
        if (tdElement != nullptr)
        {
            // Getting the a child and checking that it has the href attribute
            const GumboNode* aChild = ParserUtils::getChildWithPredicate(tdElement, GUMBO_TAG_A,
                    [](const GumboNode* node){ return ParserUtils::hasAttribute(node, "href");});
            if (aChild != nullptr)
            {
                return ParserUtils::getAttribute(aChild, "href");
            }
            else
            {
                cerr << "LiveFootballParser - The <a> element containing the link is null" << endl;
            }
        }

        return "";
    }

    // The method parses a single <tr> element containing the information for the streaming
    website::StreamingInfo LiveFootballParser::parseTrElementWithStreamingLink(const GumboNode* trElement) const
    {
        website::StreamingInfo streamingInfo;
        if (trElement != nullptr)
        {
            // Getting the <td> elements children of the <tr>
            std::vector<const GumboNode*> tdChildren = ParserUtils::getChildrenOfTag(trElement, GUMBO_TAG_TD);
            // Each child is a <td> element containing some information
            for (unsigned int tdIndex = 0; tdIndex < tdChildren.size(); ++tdIndex)
            {
                const GumboNode* tdElement = tdChildren[tdIndex];
                //TODO: The cast is ugly: check if a better solution can be found
                if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::BITRATE))
                {
                    streamingInfo.setBitRate(ParserUtils::getTextForElement(tdElement));
                }
                else if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::CHANNEL))
                {
                    // TODO: need the conversion from Windows-1251 to utf8
                    streamingInfo.setChannel(ParserUtils::getTextForElement(tdElement));
                }
                else if (tdIndex == static_cast<unsigned int>(LiveFootballParser::FieldIndex::LINK))
                {
                    streamingInfo.setLink(parseTdContainingLink(tdElement));
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
    vector<website::StreamingInfo> LiveFootballParser::parseTrListWithStreamingLinks(const vector<const GumboNode*>& trElementList) const
    {
        assert(!trElementList.empty());
        vector<website::StreamingInfo> streamingLinks;
        // Skipping the first <tr> that doesn't contain information but
        // is the header of the table (why they don't use th?)
        for (unsigned int trIndex = 1; trIndex < trElementList.size(); ++trIndex)
        {
            const GumboNode* trElement = trElementList[trIndex];
            // Adding a StreamingInfo object for each row; using move semantics
            streamingLinks.push_back(move(parseTrElementWithStreamingLink(trElement)));

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
        if (node->v.element.tag == GUMBO_TAG_A &&
            ParserUtils::hasAttribute(node, "href") &&
            isParentOfMatchLink(node->parent))
        {
            string linkOfMatch = ParserUtils::getAttribute(node, "href");
            // Checking if the link contains the name of the team
            if (linkOfMatch.find(teamName_) != string::npos)
            {
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
}
