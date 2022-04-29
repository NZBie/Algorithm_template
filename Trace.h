/*
 * @Author: NZB 
 * @Date: 2022-04-29 22:30:20 
 * @Last Modified by:   NZB 
 * @Last Modified time: 2022-04-29 22:30:20 
 */

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
