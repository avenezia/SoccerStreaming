#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

namespace network
{
    CurlHttpHandler::CurlHttpHandler(const std::string& hostnameUrl):
        hostnameUrl_(hostnameUrl)
    {

    }

    CurlHttpHandler::~CurlHttpHandler()
    {

    }

    HttpResponse CurlHttpHandler::getRequest(const std::string& requestUri,
        const std::vector<std::pair<std::string, std::string>>& headers)
    {
        std::list<std::string> headerList;
        // Converting the headers in the Curl format taking a list of strings in the form key:value
        std::transform (headers.begin(), headers.end(), std::back_inserter(headerList),
            [] (const std::pair<std::string, std::string>& header) { return header.first + ":" + header.second; });
        curlpp::options::HttpHeader httpHeaders(headerList);

        // Curl is performing the copy of the object: allocation on the heap could be used
        // in order to be more efficient
        std::stringstream responseStream;
        requestHandler_.setOpt(httpHeaders);
        requestHandler_.setOpt(curlpp::options::Url(hostnameUrl_ + requestUri));
        // The response will be written in the stringstream
        requestHandler_.setOpt(curlpp::options::WriteStream(&responseStream));
        requestHandler_.perform();

        // It seems that Curlpp doesn't provide access to response headers: skipping them
        // Returning the response by value is ok because the Return value optimization or
        // move semantics will be used, avoiding huge copies
        HttpResponse response(responseStream.str(), curlpp::infos::ResponseCode::get(requestHandler_));
        return response;
    }

    HttpResponse CurlHttpHandler::getRequest(const std::string& requestUri,
		const std::pair<std::string, std::string>& singleHeader)
    {
    	std::vector<std::pair<std::string, std::string>> v;
    	v.push_back(singleHeader);
		return getRequest(requestUri, v);
    }

    HttpResponse CurlHttpHandler::getRequest(const std::string& requestUri)
    {
        std::vector<std::pair<std::string, std::string>> v;
        return getRequest(requestUri, v);
    }
}

