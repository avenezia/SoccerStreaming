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
using namespace std;

#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>
#include <glog/logging.h>

namespace network
{
    const string CurlHttpHandler::kHttpBodyStart("<html>");

    CurlHttpHandler::CurlHttpHandler(const string& hostnameUrl):
        hostnameUrl_(hostnameUrl)
    {

    }

    CurlHttpHandler::~CurlHttpHandler()
    {

    }

    HttpResponse CurlHttpHandler::getRequest(const string& requestUri,
        const vector<pair<string, string>>& headers,
        bool withAbsolutePath)
    {
        list<string> headerList;
        // Converting the headers in the Curl format taking a list of strings in the form key:value
        transform (headers.begin(), headers.end(), back_inserter(headerList),
            [] (const pair<string, string>& header) { return header.first + ":" + header.second; });
        curlpp::options::HttpHeader httpHeaders(headerList);

        // Curl is performing the copy of the object: allocation on the heap could be used
        // in order to be more efficient
        stringstream responseStream;
        requestHandler_.setOpt(httpHeaders);
        if (!withAbsolutePath)
        {
            // Making a request considering requestUri as a relative path to the hostname
            requestHandler_.setOpt(curlpp::options::Url(hostnameUrl_ + requestUri));
        }
        else
        {
            // Making a request considering requestUri as an absolute path for the hostname
            // TODO: In this case a check on the domain is performed
            requestHandler_.setOpt(curlpp::options::Url(requestUri));
        }

        // To add the header to the resulting stringstream
        requestHandler_.setOpt(curlpp::options::Header(true));
        // The response will be written in the stringstream
        requestHandler_.setOpt(curlpp::options::WriteStream(&responseStream));
        requestHandler_.perform();
        // Splitting the header and the body from the response stream
        pair<string, string> headerBodyPair = getHeaderAndBodyFromStream(responseStream.str());
        // Returning the response by value is ok because the Return value optimization or
        // move semantics will be used, avoiding huge copies
        HttpResponse response(move(headerBodyPair.second), move(headerBodyPair.first),
                curlpp::infos::ResponseCode::get(requestHandler_));
        return response;
    }

    HttpResponse CurlHttpHandler::getRequest(const string& requestUri,
        const pair<string, string>& singleHeader,
        bool withAbsolutePath)
    {
        vector<pair<string, string>> v;
        v.push_back(singleHeader);
        return getRequest(requestUri, v, withAbsolutePath);
    }

    HttpResponse CurlHttpHandler::getRequest(const string& requestUri,
        bool withAbsolutePath)
    {
        vector<pair<string, string>> v;
        return getRequest(requestUri, v, withAbsolutePath);
    }

    // The method will separate the header and the body, putting the header as pair.first and the body
    // as pair.second. In case the separation can't be found, everything is put as body
    pair<string, string> CurlHttpHandler::getHeaderAndBodyFromStream(const string& streamString) const
    {
        pair<string, string> headerBodyPair;
        // Searching the beginning of the http response body so that we can split the header and the body.
        // This is done because it seems that curlpp doesn't provide an easy way to access the header.
        // In any case it should be performed in a more robust way because we are searching only
        // the lowercase <html> tag, and it wouldn't work in case of uppercase tag.
        size_t headerBodyIndexSeparator = streamString.find(kHttpBodyStart);
        if (headerBodyIndexSeparator != string::npos)
        {
            headerBodyPair.first = streamString.substr(0, headerBodyIndexSeparator);
            headerBodyPair.second = streamString.substr(headerBodyIndexSeparator);
        }
        else
        {
            LOG(WARNING) << "Unable to find separation between HTTP response header and body";
            headerBodyPair.first = "";
            headerBodyPair.second = streamString;
        }

        return headerBodyPair;
    }
}

