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
    RojaDirectaHandler::RojaDirectaHandler(const string& hostName):
         htmlParser_(),
         httpHandler_(new network::CurlHttpHandler(hostName))
    {

    }
    
    RojaDirectaHandler::~RojaDirectaHandler()
    {

    }

    vector<StreamingInfo> RojaDirectaHandler::getStreamingLinks(const string& teamName)
    {
        string homePageContent = performHttpRequest("/");
        vector<StreamingInfo> streamingInfoContainer = htmlParser_.getStreamingLinks(homePageContent, teamName);
        return streamingInfoContainer;
    }

    //TODO: duplicated code with LiveFootballHandler
    // The method performs an HTTP request: by default withAbsolutePath is set to false
    // so that a relative URI is expected
    string RojaDirectaHandler::performHttpRequest(const string& pageUrl,
            bool withAbsolutePath) const
    {
        network::HttpResponse httpResponse = httpHandler_->getRequest(pageUrl, withAbsolutePath);
        if (httpResponse.getStatusCode() == 200)
        {
            return httpResponse.getBody();
        }
        else
        {
            cerr << "RojaDirectaHandler - HTTP error while fetching " << pageUrl <<
                    " , status code " << httpResponse.getStatusCode() << endl;
            return "";
        }
    }
}
