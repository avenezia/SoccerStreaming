#include <curlpp/cURLpp.hpp>

#include <fstream>
#include <iostream>
#include <memory>
using namespace std;

#include <glog/logging.h>

//#include "CurlHttpHandler.hpp"
//#include "HttpResponse.hpp"
#include "LiveFootballHandler.hpp"
#include "RojaDirectaParser.hpp"
#include "SiteHandler.hpp"
#include "StreamingInfo.hpp"

string openFile(const string& fileName)
{
    string result;
    string line;
    ifstream fileStream(fileName);
    if (fileStream.is_open())
    {
        while (fileStream.good())
        {
            getline (fileStream,line);
            result += line;
        }
        fileStream.close();
    }
    else
    {
        cerr << "Unable to open file";
    }

    return result;
}

void test(char *argv[])
{
    //network::CurlHttpHandler c("http://www.rojadirecta.me/");
    //cout << c.getRequest("/").getBody();
    parser::RojaDirectaParser p;
    auto v = p.getStreamingLinks(openFile(argv[1]), argv[2]);
    for (auto& e: v)
    {
        cout << e << endl;
    }
}

int main(int argc, char *argv[])
{
    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
    if (argc == 3)
        test(argv);
    else if (argc == 2)
    {
        cURLpp::initialize();
        unique_ptr<website::SiteHandler> liveFootball = website::SiteHandler::makeSiteHandler(website::SiteHandler::SiteHandlerType::LIVEFOOTBALL);
        auto liveFootballResults = liveFootball->getStreamingLinks(argv[1]);
        for (auto iter = begin(liveFootballResults); iter != end(liveFootballResults); ++iter)
        {
            cout << *iter << endl;
        }

        unique_ptr<website::SiteHandler> rojaDirecta = website::SiteHandler::makeSiteHandler(website::SiteHandler::SiteHandlerType::ROJADIRECTA);
        auto rojaDirectaResults = rojaDirecta->getStreamingLinks(argv[1]);
        for (auto iter = begin(rojaDirectaResults); iter != end(rojaDirectaResults); ++iter)
        {
            cout << *iter << endl;
        }
    }
    else
    {
        cout << "Usage: " << argv[0] << " name_of_the_team" << endl;
    }
}
