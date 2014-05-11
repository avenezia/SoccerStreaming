#include "RojaDirectaParser.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include <cctype>
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
    const string RojaDirectaParser::kContainerDivId("agendadiv");
    const string RojaDirectaParser::kLinkPrefix("goto/");
    const string RojaDirectaParser::kSpanClass("list");
    const string RojaDirectaParser::kTableDivId("taboastreams");

    RojaDirectaParser::RojaDirectaParser():
        teamName_("")
    {

    }

    RojaDirectaParser::~RojaDirectaParser()
    {

    }

    // The method returns the list of StreamingInfo objects containing the information parsed from the web page
    // the team name must be lowercase
    vector<website::StreamingInfo> RojaDirectaParser::getStreamingLinks(const string& matchPage,
            const string& team)
    {
        assert(boost::algorithm::all(team, ::islower));
        vector<website::StreamingInfo> streamingInfoContainer;
        parsePage(matchPage);
        // Getting the first container div addressable by id
        const GumboNode* containerDiv = ParserUtils::getElementById(parseTree_->root,
                GUMBO_TAG_DIV,
                kContainerDivId);
        if (containerDiv != nullptr)
        {
            // Looking for a <span> child with class attribute set to kSpanClass value
            const GumboNode* spanWithListClass = ParserUtils::getChildWithPredicate(containerDiv, GUMBO_TAG_SPAN,
                    [](const GumboNode* divChild){ return ParserUtils::getAttribute(divChild, "class").find(kSpanClass) != string::npos; });
            if (spanWithListClass != nullptr)
            {
                // Looking for the <span> for the team (it is inside the former <span>)
                const GumboNode* spanForTeam = ParserUtils::getChildWithPredicate(spanWithListClass, GUMBO_TAG_SPAN,
                    [this, &team](const GumboNode* spanChild){ return isSpanForTeam(spanChild, team); });
                if (spanForTeam != nullptr)
                {
                    parseSpanElement(spanForTeam, streamingInfoContainer);
                }
                else
                {
                    cerr << "RojaDirectaParser - unable to find the <span> for team " << team << endl;
                }
            }
            else
            {
                cerr << "RojaDirectaParser - unable to find the <span class='" + kSpanClass + "'>"  << endl;
            }
        }
        else
        {
            cerr << "RojaDirectaParser - unable to find the container div" << endl;
        }

        return streamingInfoContainer;
    }

    // The method checks if the span passed as argument contains the info for the match of the team
    bool RojaDirectaParser::isSpanForTeam(const GumboNode* spanElement, const string& team) const
    {
        assert(spanElement != nullptr);
        // We are searching the <div> (child of the <span>) that contains a <b> element
        // with the name of the 2 teams of the match
        const GumboNode* divElement = ParserUtils::getIthChildOfTag(spanElement, 0, GUMBO_TAG_DIV);
        if (divElement != nullptr)
        {
            // We want to search the <b> (child of <div>) containing the 2 teams for the match
            // The search is performed checking that the text child of <b> contains the name of the team;
            // the text is converted to lowercase to avoid failure in the search due to different case
            const GumboNode* bChildForTeam = ParserUtils::getChildWithPredicate(divElement, GUMBO_TAG_B,
                    [&team](const GumboNode* divChild)
                    {
                        string teamNames = ParserUtils::getTextForElement(divChild);
                        boost::algorithm::to_lower(teamNames);
                        return teamNames.find(team) != string::npos;
                    });
            // In case the node is not null, we found the <span> for the team
            return bChildForTeam != nullptr;
        }
        else
        {
            cerr << "RojaDirectaParser - unable to find the <div> for team " << team << endl;
        }

        return false;
    }

    // The method is responsible of the parsing of the span containing the info for the match
    void RojaDirectaParser::parseSpanElement(const GumboNode* spanElement,
            vector<website::StreamingInfo>& resultContainer) const
    {
        assert(spanElement != nullptr);
        // Getting the class attribute of the span to use it to build the id for the <table>
        string classAttribute = ParserUtils::getAttribute(spanElement, "class");
        if (classAttribute != "")
        {
            // Searching the <table> (child of the <span>) by id
            const GumboNode* tableElement = ParserUtils::getElementById(spanElement, GUMBO_TAG_TABLE,
                    kTableDivId + classAttribute);
            if (tableElement != nullptr)
            {
                // Getting the <tbody> child
                const GumboNode* tbodyElement = ParserUtils::getIthChildOfTag(tableElement, 0, GUMBO_TAG_TBODY);
                if (tbodyElement != nullptr)
                {
                    // Getting the list of <tr> elements
                    vector<const GumboNode*> trElementList = ParserUtils::getChildrenOfTag(tbodyElement, GUMBO_TAG_TR);
                    // Starting from index 1 because the first tr is the header of the table (they don't use th)
                    for (unsigned int trIndex = 1; trIndex < trElementList.size(); ++trIndex)
                    {
                        // Parsing the data and adding it to the container of results
                        resultContainer.push_back(parseTrElement(trElementList[trIndex]));
                    }
                }
                else
                {
                    cerr << "RojaDirectaParser - unable to find the tbody" << endl;
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

    string RojaDirectaParser::parseTdWithLink(const GumboNode* tdElement) const
    {
        assert(tdElement != nullptr);
        // Getting the <a> element inside the <b>
        const GumboNode* bElement = ParserUtils::getIthChildOfTag(tdElement, 0, GUMBO_TAG_B);
        if (bElement != nullptr)
        {
            const GumboNode* aElement = ParserUtils::getIthChildOfTag(bElement, 0, GUMBO_TAG_A);
            if (aElement != nullptr)
            {
                string link = ParserUtils::getAttribute(aElement, "href");
                // Removing a prefix that sometimes is present in the link
                if (boost::starts_with(link, kLinkPrefix))
                {
                    link = link.substr(kLinkPrefix.length());
                }

                return link;
            }
            else
            {
                cerr << "RojaDirectaParser - unable to find <a> element" << endl;
            }
        }
        else
        {
            cerr << "RojaDirectaParser - unable to find <b> element" << endl;
        }

        return "";
    }

    website::StreamingInfo RojaDirectaParser::parseTrElement(const GumboNode* trElement) const
    {
        assert(trElement != nullptr);
        website::StreamingInfo streamingInfoResult;
        vector<const GumboNode*> tdElementList = ParserUtils::getChildrenOfTag(trElement, GUMBO_TAG_TD);
        for (unsigned int tdIndex = 0; tdIndex < tdElementList.size(); ++tdIndex)
        {
            const GumboNode* tdElement = tdElementList[tdIndex];
            if (tdIndex == static_cast<unsigned int>(RojaDirectaParser::FieldIndex::CHANNEL))
            {
                streamingInfoResult.setChannel(ParserUtils::getTextForElement(tdElement));
            }
            else if (tdIndex == static_cast<unsigned int>(RojaDirectaParser::FieldIndex::BITRATE))
            {
                streamingInfoResult.setBitRate(ParserUtils::getTextForElement(tdElement));
            }
            else if (tdIndex == static_cast<unsigned int>(RojaDirectaParser::FieldIndex::LINK))
            {
                streamingInfoResult.setLink(parseTdWithLink(tdElement));
            }
        }

        return streamingInfoResult;
    }
}
