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

    RojaDirectaParser::RojaDirectaParser():
        parseTree_(nullptr, gumboPtrDeleter),
        teamName_("")
    {

    }

    RojaDirectaParser::~RojaDirectaParser()
    {

    }

    // The method returns the list of StreamingInfo object containing the information parse from the web page
    vector<website::StreamingInfo> RojaDirectaParser::getStreamingLinks(const string& matchPage)
    {
        vector<website::StreamingInfo> streamingInfoContainer;
        parsePage(matchPage);

        const GumboNode* containerDiv = ParserUtils::getElementById(parseTree_->root,
                GUMBO_TAG_DIV,
                kContainerDivId);
        if (containerDiv != nullptr)
        {
            const GumboVector* divChildrenList = &containerDiv->v.element.children;
            for (unsigned int divChildIndex = 0; divChildIndex < divChildrenList->length; ++divChildIndex)
            {
                const GumboNode* divChild = static_cast<GumboNode*>(divChildrenList->data[divChildIndex]);
                if (ParserUtils::isNodeOfTypeAndTag(divChild, GUMBO_TAG_SPAN) &&
                    ParserUtils::getAttribute(divChild, "class").find("list") != string::npos)
                {
                    const GumboVector* spanChildrenList = &divChild->v.element.children;
                    for (unsigned int spanChildIndex = 0; spanChildIndex < spanChildrenList->length; ++spanChildIndex)
                    {
                        const GumboNode* spanChild = static_cast<GumboNode*>(spanChildrenList->data[spanChildIndex]);
                        if (ParserUtils::isNodeOfTypeAndTag(divChild, GUMBO_TAG_SPAN))
                        {

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

    void RojaDirectaParser::parsePage(const string& htmlPage)
    {
        parseTree_.reset(gumbo_parse(htmlPage.c_str()));
    }
}
