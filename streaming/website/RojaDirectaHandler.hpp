#ifndef ROJA_DIRECTA_HANDLER_HPP
#define ROJA_DIRECTA_HANDLER_HPP

#include <memory>

#include "RojaDirectaParser.hpp"
#include "StreamingHandler.hpp"

namespace network{ class HttpHandler; }

namespace website
{
    class StreamingInfo;

    class RojaDirectaHandler : public StreamingHandler
    {
        public:
            RojaDirectaHandler(const std::string& hostName);
            virtual ~RojaDirectaHandler();

            virtual std::vector<StreamingInfo> getStreamingLinks(const std::string& teamName);

        private:
            parser::RojaDirectaParser htmlParser_;
    };
}

#endif
