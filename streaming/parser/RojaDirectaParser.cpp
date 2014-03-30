#include "RojaDirectaParser.hpp"

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
    const std::string RojaDirectaParser::kContainerDivId("agendadiv");
    const std::string RojaDirectaParser::kSpanClass("list");
    const std::string RojaDirectaParser::kTableDivId("taboastreams");

    RojaDirectaParser::RojaDirectaParser():
        parseTree_(nullptr, gumboPtrDeleter),
        teamName_("")
    {

    }

    RojaDirectaParser::~RojaDirectaParser()
    {

    }

    // The method returns the list of StreamingInfo objects containing the information parsed from the web page
    vector<website::StreamingInfo> RojaDirectaParser::getStreamingLinks(const string& matchPage,
            const std::string& team)
    {
        vector<website::StreamingInfo> streamingInfoContainer;
        parsePage(matchPage);
        // Getting the first container div addressable by id
        const GumboNode* containerDiv = ParserUtils::getElementById(parseTree_->root,
                GUMBO_TAG_DIV,
                kContainerDivId);
        if (containerDiv != nullptr)
        {
            const GumboVector* divChildrenList = &containerDiv->v.element.children;
            for (unsigned int divChildIndex = 0; divChildIndex < divChildrenList->length; ++divChildIndex)
            {
                // Searching a span (with class list) that contains a list of span, one for each match
                const GumboNode* divChild = static_cast<GumboNode*>(divChildrenList->data[divChildIndex]);
                if (ParserUtils::isNodeOfTypeAndTag(divChild, GUMBO_TAG_SPAN) &&
                    ParserUtils::getAttribute(divChild, "class").find(kSpanClass) != string::npos)
                {
                    // Now divChild is the span element containing all the spans with the useful information
                    const GumboVector* spanChildrenList = &divChild->v.element.children;
                    for (unsigned int spanChildIndex = 0; spanChildIndex < spanChildrenList->length; ++spanChildIndex)
                    {
                        // Searching the span with the information for the match of the team
                        const GumboNode* spanChild = static_cast<GumboNode*>(spanChildrenList->data[spanChildIndex]);
                        if (ParserUtils::isNodeOfTypeAndTag(spanChild, GUMBO_TAG_SPAN) &&
                            isSpanForTeam(spanChild, team))
                        {
                            parseSpanElement(spanChild, streamingInfoContainer);
                        }
                    }
                }
            }
        }
        else
        {
            cerr << "RojaDirectaParser - unable to find the container div" << endl;
        }

        return streamingInfoContainer;
    }

    // The method checks if the span passed as argument contains the info for the match of the team
    bool RojaDirectaParser::isSpanForTeam(const GumboNode* spanElement, const std::string& team) const
    {
        assert(spanElement != nullptr);
        const GumboVector* spanChildrenList = &spanElement->v.element.children;
        // We are searching the div (child of the span) that contains a b element
        // with the 2 teams of the match
        for (unsigned int spanChildIndex = 0; spanChildIndex < spanChildrenList->length; ++spanChildIndex)
        {
            const GumboNode* spanChild = static_cast<GumboNode*>(spanChildrenList->data[spanChildIndex]);
            if (ParserUtils::isNodeOfTypeAndTag(spanChild, GUMBO_TAG_DIV))
            {
                // Now spanChild is the div: we want to search its b child containing the 2 teams for the match
                const GumboVector* divChildrenList = &spanChild->v.element.children;
                for (unsigned int divChildIndex = 0; divChildIndex < divChildrenList->length; ++divChildIndex)
                {
                    const GumboNode* divChild = static_cast<GumboNode*>(divChildrenList->data[divChildIndex]);
                    if (ParserUtils::isNodeOfTypeAndTag(divChild, GUMBO_TAG_B))
                    {
                        // Checking if the text of the b element contains the team we want
                        return ParserUtils::getTextForElement(divChild).find(team) != string::npos;
                    }
                }
            }
        }

        return false;
    }

    void RojaDirectaParser::parsePage(const string& htmlPage)
    {
        parseTree_.reset(gumbo_parse(htmlPage.c_str()));
    }

    // The method is responsible of the parsing of the span containing the info for the match
    void RojaDirectaParser::parseSpanElement(const GumboNode* spanElement,
            vector<website::StreamingInfo>& resultContainer) const
    {
        assert(spanElement != nullptr);
        // Getting the class attribute of the span to use it to build the id for the table
        string classAttribute = ParserUtils::getAttribute(spanElement, "class");
        if (classAttribute != "")
        {
            // Searching the table (child of the span) by id
            const GumboNode* tableElement = ParserUtils::getElementById(spanElement, GUMBO_TAG_TABLE,
                    kTableDivId + classAttribute);
            if (tableElement != nullptr)
            {
                const GumboVector* tableChildrenList = &tableElement->v.element.children;
                for (unsigned int tableChildIndex = 0; tableChildIndex < tableChildrenList->length; ++tableChildIndex)
                {
                    // Searching the tbody
                    const GumboNode* tableChild = static_cast<GumboNode*>(tableChildrenList->data[tableChildIndex]);
                    if (ParserUtils::isNodeOfTypeAndTag(tableChild, GUMBO_TAG_TBODY))
                    {
                        // Now tableChild is the tbody element and we have to parse all the tr children
                        const GumboVector* tbodyChildrenList = &tableChild->v.element.children;
                        for (unsigned int tbodyChildIndex = 0; tbodyChildIndex < tbodyChildrenList->length; ++tbodyChildIndex)
                        {
                            // Searching the tr elements in order to parse their information
                            const GumboNode* tbodyChild = static_cast<GumboNode*>(tbodyChildrenList->data[tbodyChildIndex]);
                            if (ParserUtils::isNodeOfTypeAndTag(tbodyChild, GUMBO_TAG_TR))
                            {
                                parseTrElement(tbodyChild);
                            }
                        }
                    }
                }
            }
            else
            {
                cerr << "RojaDirectaParser - unable to find the table by id" << endl;
            }
        }
        else
        {
            cerr << "RojaDirectaParser - the class attribute for span element is empty" << endl;
        }
    }

    void RojaDirectaParser::parseTrElement(const GumboNode* trElement) const
    {
        assert(trElement != nullptr);
        const GumboVector* trChildrenList = &trElement->v.element.children;
        for (unsigned int trChildIndex = 0; trChildIndex < trChildrenList->length; ++trChildIndex)
        {
            // Searching the children td
            const GumboNode* trChild = static_cast<GumboNode*>(trChildrenList->data[trChildIndex]);
            if (ParserUtils::isNodeOfTypeAndTag(trChild, GUMBO_TAG_TD))
            {
                if (trChildIndex == static_cast<unsigned int>(RojaDirectaParser::FieldIndex::CHANNEL))
                {
                    cout << ParserUtils::getTextForElement(trChild) << endl;
                }
            }
        }
    }
}
