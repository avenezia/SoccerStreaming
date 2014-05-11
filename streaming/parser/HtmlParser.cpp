#include "HtmlParser.hpp"

#include <memory>
#include <string>
using namespace std;

#include "gumbo.h"

#include "ParserUtils.hpp"

namespace parser
{
    HtmlParser::HtmlParser():
        parseTree_(nullptr, gumboPtrDeleter)
    {

    }

    HtmlParser::~HtmlParser()
    {

    }

    void HtmlParser::parsePage(const string& htmlPage)
    {
        parseTree_.reset(gumbo_parse(htmlPage.c_str()));
    }
}
