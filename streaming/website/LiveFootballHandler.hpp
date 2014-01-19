#ifndef LIVE_FOOTBALL_HANDLER_HPP
#define LIVE_FOOTBALL_HANDLER_HPP

#include "StreamingHandler.hpp"

namespace website
{
	class LiveFootballHandler
	{
		public:
			LiveFootballHandler(const std::string& hostName);
			virtual ~LiveFootballHandler();

			virtual std::vector<std::string> getStreamingLinks(const std::string& teamName);

		private:

	};
}

#endif