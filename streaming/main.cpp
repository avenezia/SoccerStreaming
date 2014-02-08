#include "LiveFootballHandler.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>
using namespace std;

int main()
{
	/*
    network::CurlHttpHandler test("http://livefootball.ws/");
    vector<pair<string, string>> headers;
    headers.push_back(make_pair("user-agent", "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.77 Safari/537.36"));
    headers.push_back(std::make_pair("Cookie", "unddos=96044b7c65e3ed067353592934c89219"));
	network::HttpResponse response = test.getRequest("/", headers);
    cout << "Response Code " << response.getStatusCode() << endl;
    cout << response.getBody() << endl;
    */
	website::LiveFootballHandler handler("http://livefootball.ws/");
	handler.getStreamingLinks("");
}
