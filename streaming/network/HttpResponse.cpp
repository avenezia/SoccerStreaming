#include "HttpResponse.hpp"

#include <utility>

namespace network
{
	HttpResponse::HttpResponse(const std::string& body, const std::string& header, short statusCode):
			body_(body),
			header_(header),
			statusCode_(statusCode)
	{

	}

	HttpResponse::HttpResponse(const std::string& body, short statusCode):
			HttpResponse(body, "", statusCode)
	{

	}

	HttpResponse::HttpResponse(HttpResponse&& h):
			body_(std::move(h.body_)),
			header_(std::move(h.header_)),
			statusCode_(std::move(h.statusCode_))
	{

	}

	const std::string& HttpResponse::getBody() const
	{
		return body_;
	}

	const std::string& HttpResponse::getHeader() const
	{
		return header_;
	}

	short HttpResponse::getStatusCode() const
	{
		return statusCode_;
	}
}
