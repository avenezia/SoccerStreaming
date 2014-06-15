#include "StreamingInfoManager.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <cassert>
#include <utility>
using namespace std;

#include "SiteHandler.hpp"
#include "StreamingInfo.hpp"

namespace website
{
    StreamingInfoManager::StreamingInfoManager():
        // Reserving the space for the handlers in the different vectors
        siteHandlers_(static_cast<int>(SiteHandler::SiteHandlerType::SITE_HANDLER_COUNT)),
        siteHandlerResults_(static_cast<int>(SiteHandler::SiteHandlerType::SITE_HANDLER_COUNT)),
        siteHandlersThreads_(static_cast<int>(SiteHandler::SiteHandlerType::SITE_HANDLER_COUNT))
    {
        siteHandlers_.push_back(SiteHandler::makeSiteHandler(SiteHandler::SiteHandlerType::LIVEFOOTBALL));
        siteHandlers_.push_back(SiteHandler::makeSiteHandler(SiteHandler::SiteHandlerType::ROJADIRECTA));
    }

    StreamingInfoManager::~StreamingInfoManager()
    {
        for (auto& thread : siteHandlersThreads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void StreamingInfoManager::searchStreamingLinksForTeam(const std::string& team)
    {
        teamName_ = team;
        // Converting to lowercase to avoid issues in the web page parsing
        boost::algorithm::to_lower(teamName_);
        for (size_t handlerIndex = 0; handlerIndex < siteHandlers_.size(); ++handlerIndex)
        {
            siteHandlersThreads_.push_back(thread(&StreamingInfoManager::siteHandlerThreadMethod, this, handlerIndex));
        }
    }

    void StreamingInfoManager::siteHandlerThreadMethod(size_t index)
    {
        // TODO: exception must be catched otherwise the program will be terminated
        assert(index < siteHandlersThreads_.size());
        vector<StreamingInfo> handlerResult = siteHandlers_[index]->getStreamingLinks(teamName_);
        lock_guard<mutex> lock(resultsMutex_);
        siteHandlerResults_.push_back(move(handlerResult));
        resultsConditionVariable_.notify_all();
    }
}
