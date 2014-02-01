#ifndef STREAMING_INFO_HPP
#define STREAMING_INFO_HPP

#include <string>

namespace website
{
    class StreamingInfo
    {
        public:
            StreamingInfo(const std::string& link, const std::string& channel, const std::string& bitrate);
            ~StreamingInfo() = default;

            const std::string& getBitRate() const;
            const std::string& getChannel() const;
            const std::string& getLink() const;

        private:
            std::string link_;
            std::string channel_;
            std::string bitRate_;
    };
}

#endif