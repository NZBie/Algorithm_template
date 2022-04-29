#ifndef TRACE_H
#define TRACE_H

#include <cstdio>
#include <iostream>
#include <string>

class Trace {
public:
    Trace(bool isOut = true): _isOut(isOut) {};
    // void debug(char* out) {
    //     printf("%s\n", out);
    // }
    void print(std::string out) {
        if(_isOut) std::cout<<out<<std::endl;
    }
    // void setIsOut() { _isOut = false; }
private:
    bool _isOut;
};

#endif
