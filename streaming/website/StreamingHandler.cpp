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
            return unique_ptr<StreamingHandler>(new LiveFootballHandler("http://livefootball.ws/"));
        }
        else if (type == StreamingHandlerType::ROJADIRECTA)
        {
            return unique_ptr<StreamingHandler>(new RojaDirectaHandler("http://www.rojadirecta.me/"));
        }
        else
        {
            // It should never happen
            // TODO: add exception
        }

        return nullptr;
    }
}
