#include <vector>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>

std::string get_env_var(std::string const &key)
{
    char *val = getenv(key.c_str());
    std::string retval = "";

    if (val != NULL)
    {
        retval = val;
    }

    return retval;
}

// need to make output a string by yourself
int file_write(std::string path, std::string output)
{
    int has_error = 1; // negativity everywhere

    std::ofstream stream;
    stream.open(path);

    if (stream.is_open())
    {
        stream << output << std::endl;
        stream.close();
        has_error = 0;
    }
    else
    {
        std::cerr << "\n\nstream cannot be found\n\n";
    }

    return has_error;
}

std::vector<std::string> get_env_all(char *arge[])
{
    std::vector<std::string> vec;

    int i = 0;
    while (arge[i] != NULL)
    {
        vec.push_back(arge[i]);
        i++;
    }

    return vec;
}

// int possibly_print_working_directory()
// {
// #include <stdio.h> /* defines FILENAME_MAX */
// #ifdef WINDOWS
// #include <direct.h>
// #define GetCurrentDir _getcwd
// #else
// #include <unistd.h>
// #define GetCurrentDir getcwd
// #endif

//     char cCurrentPath[FILENAME_MAX];

//     if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
//     {
//         return errno;
//     }

//     cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

//     printf("The current working directory is %s", cCurrentPath);
// }

int main(int argc, char *argv[], char *arge[])
{
    std::vector<std::string> vec = get_env_all(arge);

    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << vec.at(i) << std::endl;
    }

    return 0;
}