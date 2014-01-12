#include "CurlHttpHandler.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

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

    short CurlHttpHandler::handleGet(const std::string& requestUri, 
        const std::vector<std::pair<std::string, std::string>>& headerList,
        std::string& httpResponseBody)
    {
        //TODO: handle headers and http response code
        requestHandler_.setOpt(curlpp::options::Url(hostnameUrl_ + requestUri));
        requestHandler_.perform();
        stringstream stringStream;
        stringStream << requestHandler_;
        httpResponseBody = stringStream.str();
    }

    short CurlHttpHandler::handleGet(const std::string& requestUri, 
        std::string& httpResponseBody)
    {
        std::vector<std::pair<std::string, std::string>> v;
        handleGet(requestUri, v, httpResponseBody);
    }
}

