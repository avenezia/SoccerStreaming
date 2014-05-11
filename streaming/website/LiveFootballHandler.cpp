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
    const boost::regex LiveFootballHandler::kAccessCookieRegExp("antid=[a-z0-9]+; path=/");

    LiveFootballHandler::LiveFootballHandler(const string& hostName)
        :accessCookie_(""),
         htmlParser_(),
         httpHandler_(new network::CurlHttpHandler(hostName))
    {

    }
    
    LiveFootballHandler::~LiveFootballHandler()
    {

    }

    /// The method performs an HTTP request in order to get the access cookie
    /// useful to get the real home page of the website
    void LiveFootballHandler::setAccessCookieFromHomePage()
    {
        network::HttpResponse homePageResponse = httpHandler_->getRequest("/");
        if (homePageResponse.getStatusCode() == 307)
        {
            // Parsing the cookie useful to get the real home page
            accessCookie_ = parseAccessCookie(homePageResponse.getHeader());
        }
        else
        {
            // TODO: use a better way of logging
            cerr << "LiveFootballHandler - HTTP error while fetching home, status code " <<
                    homePageResponse.getStatusCode() << endl;
        }
    }

    // The method gets the page for the match, performing an HTTP request
    // with an absolute path since the link is parsed from the home page
    string LiveFootballHandler::getMatchPage(const string& matchPageLink) const
    {
        return performHttpRequest(matchPageLink, true);
    }

    // The method gets the home page of the website, performing an HTTP request
    // with the access cookie
    string LiveFootballHandler::getRealHomePage() const
    {
        if (accessCookie_ != "")
        {
            return performHttpRequest("/");
        }
        else
        {
            // TODO: use a better way of logging
            cerr << "LiveFootballHandler: unable to get the access cookie";
            return "";
        }
    }

    vector<StreamingInfo> LiveFootballHandler::getStreamingLinks(const string& teamName)
    {
        // If we don't provide the access cookie in the first request, the site will provide
        // us a page containing only few javascript and the access cookie; for this reason
        // the method setAccessCookieFromHomePage makes a first request to get the cookie and
        // then getRealHomePage will make the request for the real home page
        setAccessCookieFromHomePage();
        string homePage = getRealHomePage();
        // We are looking for the link to the web page containing the details of the streaming
        // for the match with team teamName
        vector<StreamingInfo> streamingInfoContainer;
        string linkForMatchPage = htmlParser_.getLinkForTeamMatch(homePage, teamName);
        if (linkForMatchPage != "")
        {
            // Performing the HTTP request to fetch the page for the match
            string matchPageContent = getMatchPage(linkForMatchPage);
            if (matchPageContent != "")
            {
                // Parsing the page to get the StreamingInfo objects
                return htmlParser_.getStreamingLinks(matchPageContent);
            }
        }
        else
        {
            cerr << "LiveFootballHandler: unable to find the link to the match with " << teamName << endl;
        }

        return streamingInfoContainer;
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

    // The method performs an HTTP request: by default withAbsolutePath is set to false
    // so that a relative URI is expected
    string LiveFootballHandler::performHttpRequest(const string& pageUrl,
            bool withAbsolutePath) const
    {
        network::HttpResponse httpResponse = httpHandler_->getRequest(pageUrl,
                make_pair("Cookie", accessCookie_), withAbsolutePath);
        if (httpResponse.getStatusCode() == 200)
        {
            return httpResponse.getBody();
        }
        else
        {
            cerr << "LiveFootballHandler - HTTP error while fetching " << pageUrl <<
                    " , status code " << httpResponse.getStatusCode() << endl;
            return "";
        }
    }
}
