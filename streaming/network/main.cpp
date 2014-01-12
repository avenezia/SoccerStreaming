#include "CurlHttpHandler.hpp"

#include <iostream>
using namespace std;

int main()
{
    network::CurlHttpHandler test("http://livefootball.ws/");   
    std::string res;
    test.handleGet("/", res);
    cout << res << endl;
}