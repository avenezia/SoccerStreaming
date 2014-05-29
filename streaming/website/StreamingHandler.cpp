#include "StreamingHandler.hpp"

#include <memory>
using namespace std;

#include <glog/logging.h>

#include "HttpResponse.hpp"
#include "LiveFootballHandler.hpp"
#include "RojaDirectaHandler.hpp"

namespace website
{
    // In C++14 make_unique would be the solution
    unique_ptr<StreamingHandler> StreamingHandler::makeStreamingHandler(StreamingHandlerType type)
    {
        if (type == StreamingHandlerType::LIVEFOOTBALL)
        {
            return unique_ptr<StreamingHandler>(new LiveFootballHandler("http://livefootball.ws/"));
        }
        else if (type == StreamingHandlerType::ROJADIRECTA)
        {
            return unique_ptr<StreamingHandler>(new RojaDirectaHandler("http://www.rojadirecta.me/"));
        }
        else
        {
            // It should never happen
            throw UnknownStreamingHandler();
        }

        return nullptr;
    }

    // Utility method to avoid to pass the header in case is not needed
    string StreamingHandler::performHttpRequest(const string& pageUrl,
            bool withAbsolutePath) const
    {
        pair<string, string> emptyHeader;
        return performHttpRequest(pageUrl, withAbsolutePath, emptyHeader);
    }

    // The method performs an HTTP request: if withAbsolutePath is true
    // an absolute URI is expected; a relative one otherwise.
    // It returns the body of the response if the status is 200;
    // otherwise an empty string, writing an error in the logs
    // TODO: only one header could not be enough, even if now it's ok
    string StreamingHandler::performHttpRequest(const string& pageUrl,
            bool withAbsolutePath,
            const pair<string, string>& header) const
    {
        network::HttpResponse httpResponse = httpHandler_->getRequest(pageUrl, header, withAbsolutePath);
        if (httpResponse.getStatusCode() == 200)
        {
            return httpResponse.getBody();
        }
        else
        {
            LOG(WARNING) << "RojaDirectaHandler - HTTP error while fetching " << pageUrl <<
                    " , status code " << httpResponse.getStatusCode();
        }

        return "";
    }
}
