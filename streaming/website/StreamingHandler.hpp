#ifndef STREAMING_HANDLER_HPP
#define STREAMING_HANDLER_HPP

#include <memory>
#include <string>
#include <vector>

namespace network{ class HttpHandler; }

namespace website
{
	class StreamingHandler
	{
		public:
			StreamingHandler(const std::string& hostName);
			virtual ~StreamingHandler();

			virtual std::vector<std::string> getStreamingLinks(const std::string& teamName) = 0;

		private:
			std::unique_ptr<network::HttpHandler> httpHandler_;

	};
}

#endif