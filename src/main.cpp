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

/*
    To handle request for portability here are the three main OS.
    each '#ifdef', '#endif' pair corresponds to a different OS
    and returns a method 'getexepath()' unique to that OS so
    I can call getexepath freely elsewhere in my code.
*/
#ifdef _WIN32 // includes both x32 and x64
std::string getexepath()
{
    char result[MAX_PATH];
    // need to use ansi version (post-pend 'A') otherwise char* incompatible...
    return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}
#endif
#ifdef __linux__
#include <unistd.h>
std::string getexepath()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}
#endif
#ifdef __APPLE__
// more to come
#endif

int main(int argc, char *argv[], char *arge[])
{
    std::vector<std::string> env_args;
    env_args.assign(arge, arge + 62);

    return 0;
}