SoccerStreaming
===============
This is a project developed to test some new features of C++11 like move semantics, Rvalue references, smart pointers, threads etc. The aim is to fetch from some internet sites the information for the streaming of football matches for a team requested by the user. The project uses some external libraries: 
- Google gumbo-parser (https://github.com/google/gumbo-parser), to parse the HTML
- Curlpp (https://code.google.com/p/curlpp/), a C++ wrapper for libcURL used for networking
- Boost (http://www.boost.org/) for regular expressions (not yet fully implemented in g++), string utilities etc.

Scons is used as alternative to Make
