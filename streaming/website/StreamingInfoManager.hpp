#ifndef STREAMING_INFO_MANAGER_HPP_
#define STREAMING_INFO_MANAGER_HPP_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace website
{
    class SiteHandler;
    class StreamingInfo;

    class StreamingInfoManager
    {
        public:
            StreamingInfoManager();
            ~StreamingInfoManager();

            void searchStreamingLinksForTeam(const std::string& team);

        private:
            void siteHandlerThreadMethod(size_t index);

            std::condition_variable resultsConditionVariable_;
            std::mutex resultsMutex_;
            std::vector<std::unique_ptr<SiteHandler>> siteHandlers_;
            std::vector<std::vector<StreamingInfo>> siteHandlerResults_;
            std::vector<std::thread> siteHandlersThreads_;
            std::string teamName_;
    };

}
#endif
