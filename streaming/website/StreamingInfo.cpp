#include "StreamingInfo.hpp"

namespace website
{
    StreamingInfo::StreamingInfo(const std::string& link, const std::string& channel, const std::string& bitrate):
		link_(link),
		channel_(channel),
		bitRate_(bitrate)
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
}
