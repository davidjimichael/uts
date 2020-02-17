#ifndef LIB_H_
#define LIB_H_

#include <limits.h>
#include <string>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>

namespace lib
{
// number of environment args, used for dump copying arge on init

/*
    To handle request for portability here are the three main OS.
    each '#ifdef', '#endif' pair corresponds to a different OS
    and returns a method 'getexepath()' unique to that OS so
    I can call getexepath freely elsewhere in my code.
*/
#ifdef _WIN32 // includes both x32 and x64
const std::string FAV_SEP = "\\";
const int ENV_SIZE = 62;
std::string getexepath()
{
    char result[MAX_PATH];
    // need to use ansi version (post-pend 'A') otherwise char* incompatible...
    return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}
#endif

#ifdef __linux__
#include <unistd.h>
const std::string FAV_SEP = "/";
// todo test this number
const int ENV_SIZE = 62;
std::string getexepath()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}
#endif

#ifdef __APPLE__
const std::string FAV_SEP = "/";
// todo test this number
const int ENV_SIZE = 62;
std::string getexepath()
{
    std::cerr << "I have not implemented this method on OSX";
    return "";
}
#endif
} // namespace lib

#endif // LIB_H_