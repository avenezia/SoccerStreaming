#ifndef STREAMING_INFO_HPP
#define STREAMING_INFO_HPP

#include <iosfwd>
#include <string>

namespace website
{
    class StreamingInfo
    {
        public:
            StreamingInfo(const std::string& link, const std::string& channel, const std::string& bitrate);
            StreamingInfo();
            StreamingInfo(StreamingInfo&& s);
            ~StreamingInfo() = default;

            const std::string& getBitRate() const;
            const std::string& getChannel() const;
            const std::string& getLink() const;

            void setBitRate(const std::string& bitRate);
            void setChannel(const std::string& channel);
            void setLink(const std::string& link);

            friend std::ostream& operator<<(std::ostream& o, const StreamingInfo& streamingInfo);

        private:
            std::string link_;
            std::string channel_;
            std::string bitRate_;
    };
}

#endif
