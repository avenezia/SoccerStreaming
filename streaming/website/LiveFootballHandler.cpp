#include "LiveFootballHandler.hpp"
#include "network/CurlHttpHandler.hpp"

#include <string>
#include <vector>


namespace website
{
    class LiveFootballHandler
    {
    
        LiveFootballHandler::LiveFootballHandler(const std::string& hostName)
            :httpHandler_(new network::CurlHttpHandler(hostName))
        {

        }
        
        LiveFootballHandler::~LiveFootballHandler()
        {

        }

        std::vector<std::string> LiveFootballHandler::getStreamingLinks(const std::string& teamName)
        {
            std::vector<std::string> result;
            return result;
        }
    };
}

#endif