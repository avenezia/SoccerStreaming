#include "HttpResponse.hpp"

#include <string>
#include <utility>
using namespace std;

namespace network
{
	HttpResponse::HttpResponse(const string& body, const string& header, short statusCode):
			body_(body),
			header_(header),
			statusCode_(statusCode)
	{

	}

	HttpResponse::HttpResponse(const string& body, short statusCode):
			HttpResponse(body, "", statusCode)
	{

	}

	HttpResponse::HttpResponse(string&& body, string&& header, short statusCode):
	        body_(move(body)),
	        header_(move(header)),
	        statusCode_(statusCode)
	{

	}

	HttpResponse::HttpResponse(HttpResponse&& h):
			body_(move(h.body_)),
			header_(move(h.header_)),
			statusCode_(move(h.statusCode_))
	{

	}

	const string& HttpResponse::getBody() const
	{
		return body_;
	}

	const string& HttpResponse::getHeader() const
	{
		return header_;
	}

	short HttpResponse::getStatusCode() const
	{
		return statusCode_;
	}
}
