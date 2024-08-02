#include <Util.hpp>
#include <Logger.hpp>

void assert(bool assertion) {
    assert(assertion, "Assertion error!");
}

void assert(bool assertion, std::string msg) {
    if(!assertion) {
        std::cout << msg << std::endl;
        exit(-1);
    }
}

bool is_number(std::string str) {
    for (const char c : str)
    {
        if(c < '0' || c > '9')
            return false;    
    }
    return true;
}

Logger logger;