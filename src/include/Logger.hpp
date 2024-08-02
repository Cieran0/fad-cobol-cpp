#pragma once
#include <iostream>
#include <memory>
#include <sstream>

//#define DEBUG 1

class Logger {
public:
    Logger() {
        #ifdef DEBUG
            outStream = &std::cout;
        #else
            outStream = &nullStream;
        #endif
    }

    template <typename T>
    Logger& operator<<(const T& value) {
        (*outStream) << value;
        return *this;
    }

    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        (*outStream) << manip;
        return *this;
    }

private:
    std::ostream* outStream;
    std::ostringstream nullStream;
};

