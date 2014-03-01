#include "StreamingInfo.hpp"

#include <utility>

namespace website
{
    StreamingInfo::StreamingInfo(const std::string& link, const std::string& channel, const std::string& bitrate):
		link_(link),
		channel_(channel),
		bitRate_(bitrate)
	{

	}

    StreamingInfo::StreamingInfo():
        link_(""),
        channel_(""),
        bitRate_("")
    {

    }

    StreamingInfo::StreamingInfo(StreamingInfo&& s):
        link_(std::move(s.link_)),
        channel_(std::move(s.channel_)),
        bitRate_(std::move(s.bitRate_))
    {

    }

    const std::string& StreamingInfo::getBitRate() const
    {
    	return bitRate_;
    }

    const std::string& StreamingInfo::getChannel() const
    {
    	return channel_;
    }

    const std::string& StreamingInfo::getLink() const
    {
    	return link_;
    }

    void StreamingInfo::setBitRate(const std::string& bitRate)
    {
        bitRate_ = bitRate;
    }

    void StreamingInfo::setChannel(const std::string& channel)
    {
        channel_ = channel;
    }

    void StreamingInfo::setLink(const std::string& link)
    {
        link_ = link;
    }
}
