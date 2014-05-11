#include "StreamingHandler.hpp"

#include <memory>
using namespace std;

#include "LiveFootballHandler.hpp"
#include "RojaDirectaHandler.hpp"

namespace website
{
    // In C++14 make_unique would be the solution
    unique_ptr<StreamingHandler> StreamingHandler::makeStreamingHandler(StreamingHandlerType type)
    {
        if (type == StreamingHandlerType::LIVEFOOTBALL)
        {
            unique_ptr<StreamingHandler> handlerPtr(new LiveFootballHandler("http://livefootball.ws/"));
            return handlerPtr;
        }
        else if (type == StreamingHandlerType::ROJADIRECTA)
        {
            unique_ptr<StreamingHandler> handlerPtr(new RojaDirectaHandler("http://www.rojadirecta.me/"));
            return handlerPtr;
        }
        else
        {
            // It should never happen
            // TODO: add exception
        }

        return nullptr;
    }
}
