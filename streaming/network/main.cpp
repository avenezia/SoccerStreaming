#include "CurlHttpHandler.hpp"

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
    std::string res;
    cout << "Response Code " << test.getRequest("/", headers, res) << endl;
    cout << res << endl;
}
