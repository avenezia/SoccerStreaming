#include "RojaDirectaHandler.hpp"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"
#include "RojaDirectaParser.hpp"
#include "StreamingInfo.hpp"

namespace website
{    
    RojaDirectaHandler::RojaDirectaHandler(const string& hostName)
    {
        httpHandler_.reset(new network::CurlHttpHandler(hostName));
    }
    
    RojaDirectaHandler::~RojaDirectaHandler()
    {

    }

    vector<StreamingInfo> RojaDirectaHandler::getStreamingLinks(const string& teamName)
    {
        string homePageContent = performHttpRequest("/", false);
        vector<StreamingInfo> streamingInfoContainer = htmlParser_.getStreamingLinks(homePageContent, teamName);
        return streamingInfoContainer;
    }
}
