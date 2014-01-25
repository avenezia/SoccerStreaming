#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"
#include "LiveFootballHandler.hpp"
#include "StreamingInfo.hpp"

//TODO: remove it
#include <iostream>
#include <string>
#include <vector>


namespace website
{    
    LiveFootballHandler::LiveFootballHandler(const std::string& hostName)
        :httpHandler_(new network::CurlHttpHandler(hostName))
    {

    }
    
    LiveFootballHandler::~LiveFootballHandler()
    {

    }

    std::vector<StreamingInfo> LiveFootballHandler::getStreamingLinks(const std::string& teamName)
    {
    	network::HttpResponse response = httpHandler_->getRequest("/");
    	std::cout << "Status code " << response.getStatusCode() << std::endl;
    	std::cout << response.getBody() << std::endl;
        std::vector<StreamingInfo> result;
        return result;
    }    
}
