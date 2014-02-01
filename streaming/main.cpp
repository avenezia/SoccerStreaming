#include "CurlHttpHandler.hpp"
#include "HttpResponse.hpp"

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
    network::CurlHttpHandler test("http://livefootball.ws/");
    vector<pair<string, string>> headers;
    headers.push_back(make_pair("user-agent", "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.77 Safari/537.36")); 
	network::HttpResponse response = test.getRequest("/", headers);
    cout << "Response Code " << response.getStatusCode() << endl;
    cout << response.getBody() << endl;
}
