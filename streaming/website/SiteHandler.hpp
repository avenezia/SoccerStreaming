#ifndef SITE_HANDLER_HPP
#define SITE_HANDLER_HPP

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "HttpHandler.hpp"


namespace website
{
	class StreamingInfo;

    class SiteHandler
    {
        public:
	        enum class SiteHandlerType
	        {
	            LIVEFOOTBALL,
	            ROJADIRECTA,
	            SITE_HANDLER_COUNT // Not so clean but useful to have the number of handlers
	        };

            SiteHandler() = default;
            virtual ~SiteHandler() = default;

            virtual std::vector<StreamingInfo> getStreamingLinks(const std::string& teamName) = 0;
            static std::unique_ptr<SiteHandler> makeSiteHandler(SiteHandlerType type);

        protected:
            std::string performHttpRequest(const std::string& pageUrl,
                    bool withAbsolutePath) const;
            std::string performHttpRequest(const std::string& pageUrl,
                    bool withAbsolutePath,
                    const std::pair<std::string, std::string>& header) const;

            std::unique_ptr<network::HttpHandler> httpHandler_;
    };

    class UnknownSiteHandler : public std::exception
    {
        const char* what() const noexcept override
        {
            return "Unknown Handler for streaming";
        }
    };
}

#endif
