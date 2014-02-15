#include "LiveFootballHandler.hpp"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <boost/regex.hpp>

#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"
#include "StreamingInfo.hpp"

namespace website
{    
    const boost::regex LiveFootballHandler::kAccessCookieRegExp("unddos=[0-9a-z]+");

    LiveFootballHandler::LiveFootballHandler(const string& hostName)
        :accessCookie_(""),
         htmlParser_(),
         httpHandler_(new network::CurlHttpHandler(hostName))
    {

    }
    
    LiveFootballHandler::~LiveFootballHandler()
    {

    }

    void LiveFootballHandler::setAccessCookieFromHomePage()
    {
        network::HttpResponse homePageResponse = httpHandler_->getRequest("/");
        if (homePageResponse.getStatusCode() == 200 )
        {
            // Getting the cookie useful to get the real home page
            accessCookie_ = parseAccessCookie(homePageResponse.getBody());
        }
        else
        {
            // TODO: use a better way of logging
            cerr << "LiveFootball - HTTP error while fetching home, status code " <<
                    homePageResponse.getStatusCode() << endl;
        }
    }

    string LiveFootballHandler::getRealHomePage() const
    {
        if (accessCookie_ != "")
        {
            network::HttpResponse realHomePageResponse = httpHandler_->getRequest("/",
                    make_pair("Cookie", accessCookie_));
            if (realHomePageResponse.getStatusCode() == 200)
            {
                return realHomePageResponse.getBody();
            }
            else
            {
                cerr << "LiveFootball - HTTP error while fetching home, status code " <<
                        realHomePageResponse.getStatusCode() << endl;
            }
        }
        else
        {
            // TODO: use a better way of logging
            cerr << "LiveFootball: unable to get the access cookie";
        }

        return "";
    }

    vector<StreamingInfo> LiveFootballHandler::getStreamingLinks(const string& teamName)
    {
        vector<StreamingInfo> result;
        setAccessCookieFromHomePage();
        string homePage = getRealHomePage();
        std::string linkForMatchInfo = htmlParser_.getLinkForTeamMatch(homePage, teamName);
        cout << linkForMatchInfo << endl;
        return result;
    }

    string LiveFootballHandler::parseAccessCookie(const string& webPage) const
    {
        boost::smatch regExpMatch;
        if (boost::regex_search(webPage, regExpMatch, kAccessCookieRegExp))
        {
            return regExpMatch[0].str();
        }

        return "";
    }
}
