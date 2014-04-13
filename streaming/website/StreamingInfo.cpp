#include "StreamingInfo.hpp"

#include <iostream>
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

    StreamingInfo& StreamingInfo::operator=(StreamingInfo&& s)
    {
        link_ = std::move(s.link_);
        channel_ = std::move(s.channel_);
        bitRate_ = std::move(s.bitRate_);
        return *this;
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

    void StreamingInfo::setBitRate(std::string&& bitRate)
    {
        bitRate_ = std::move(bitRate);
    }

    void StreamingInfo::setChannel(std::string&& channel)
    {
        channel_ = std::move(channel);
    }

    void StreamingInfo::setLink(std::string&& link)
    {
        link_ = std::move(link);
    }

    std::ostream& operator<<(std::ostream& o, const StreamingInfo& streamingInfo)
    {
        o << "Link ";
        o.width(50);
        o << streamingInfo.link_ << ", channel ";
        o.width(20);
        o << streamingInfo.channel_ << ", bitrate ";
        o.width(5);
        o << streamingInfo.bitRate_ << " ";
        return o;
    }
}
