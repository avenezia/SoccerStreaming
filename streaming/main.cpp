#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"


#include "LiveFootballHandler.hpp"
#include "LiveFootballParser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>
using namespace std;

string openFile(const std::string& fileName)
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

int main(int argc, char *argv[])
{
    /*
    network::CurlHttpHandler test("http://livefootball.ws/");
    vector<pair<string, string>> headers;
    headers.push_back(make_pair("user-agent", "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.77 Safari/537.36"));
    headers.push_back(std::make_pair("Cookie", "unddos=96044b7c65e3ed067353592934c89219"));
    network::HttpResponse response = test.getRequest("/5224-leverkusen-schalke.html", headers);
    cout << "Response Code " << response.getStatusCode() << endl;
    cout << response.getBody() << endl;*/

    if (argc == 2)
    {
        //website::LiveFootballHandler handler("http://livefootball.ws/");
        //handler.getStreamingLinks(argv[1]);
        string webPage(openFile(argv[1]));
        //cout << webPage.length() << endl;
        parser::LiveFootballParser parser;
        parser.getStreamingLinks(webPage);
    }
}
