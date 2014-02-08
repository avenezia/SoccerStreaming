#include "LiveFootballHandler.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <boost/regex.hpp>

#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"
#include "StreamingInfo.hpp"

namespace website
{    
	const boost::regex LiveFootballHandler::kAccessCookieRegExp("unddos=[0-9a-z]+");

    LiveFootballHandler::LiveFootballHandler(const std::string& hostName)
        :httpHandler_(new network::CurlHttpHandler(hostName))
    {

    }
    
    LiveFootballHandler::~LiveFootballHandler()
    {

    }

    std::string LiveFootballHandler::getAccessCookieFromHomePage() const
    {
		network::HttpResponse homePageResponse = httpHandler_->getRequest("/");
		if (homePageResponse.getStatusCode() == 200 )
		{
			// Getting the cookie useful to get the real home page
			return parseAccessCookie(homePageResponse.getBody());
		}
		else
		{
			// TODO: use a better way of logging
			std::cerr << "LiveFootball - HTTP error while fetching home, status code " <<
					homePageResponse.getStatusCode() << std::endl;
		}

		return "";
    }

    std::string LiveFootballHandler::getRealHomePage(const std::string& accessCookie) const
    {
    	if (accessCookie != "")
		{
			network::HttpResponse realHomePageResponse = httpHandler_->getRequest("/",
					std::make_pair("Cookie", accessCookie));
			if (realHomePageResponse.getStatusCode() == 200)
			{
				return realHomePageResponse.getBody();
			}
			else
			{
				std::cerr << "LiveFootball - HTTP error while fetching home, status code " <<
						realHomePageResponse.getStatusCode() << std::endl;
			}
		}
		else
		{
			// TODO: use a better way of logging
			std::cerr << "LiveFootball: unable to get the access cookie";
		}

    	return "";
    }

    std::vector<StreamingInfo> LiveFootballHandler::getStreamingLinks(const std::string& teamName)
    {
    	std::vector<StreamingInfo> result;
    	std::string accessCookie = getAccessCookieFromHomePage();
    	std::cout << getRealHomePage(accessCookie);

        return result;
    }

    std::string LiveFootballHandler::parseAccessCookie(const std::string& webPage) const
	{
		boost::smatch regExpMatch;
		if (boost::regex_search(webPage, regExpMatch, kAccessCookieRegExp))
		{
			return regExpMatch[0].str();
		}

		return "";
	}
}
