#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include <string>

namespace network
{
	class HttpResponse
	{
		public:
			HttpResponse(const std::string& body, const std::string& header, short statusCode);
			HttpResponse(const std::string& body, short statusCode);
			HttpResponse(HttpResponse&& h);
			~HttpResponse() = default;

			const std::string& getBody() const;
			const std::string& getHeader() const;
			short getStatusCode() const;

		private:
			std::string body_;
			std::string header_;
			short statusCode_;
	};
}

#endif
