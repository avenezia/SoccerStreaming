#include <curlpp/cURLpp.hpp>

#include "LiveFootballHandler.hpp"

#include <fstream>
#include <iostream>
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
    if (argc == 2)
    {
        cURLpp::initialize();
        website::LiveFootballHandler handler("http://livefootball.ws/");
        auto results = handler.getStreamingLinks(argv[1]);
        for (auto iter = begin(results); iter != end(results); ++iter)
        {
            cout << *iter << endl;
        }
    }
    else
    {
        cout << "Usage: " << argv[0] << " name_of_the_team" << endl;
    }
}
