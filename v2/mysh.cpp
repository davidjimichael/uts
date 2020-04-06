#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

#define MAXIN 512
#define MAXARGL 256
#define clear() printf("\033[H\033[J")

using std::map;
using std::string;
using std::vector;

void __exec__(char **argv);

map<string, string> env;
map<string, vector<string>> aliases; // permit multiple aliases

int main()
{
    int argc;
    char *argv[MAXARGL];
    char input[MAXIN];

    bool running = true;
    while (running)
    {
        // prompt
        memset(input, '\0', MAXIN);
        fgets(input, MAXIN, stdin);

        // skip empty input
        if ((argv[0] = strtok(input, " \n\t")) == nullptr)
        {
            continue;
        }

        // check exit condition
        if (strcmp(argv[0], "exit") == 0)
        {
            running = false;
            continue;
        }

        // read in args
        argc = 1;
        while ((argv[argc] = strtok(nullptr, " \n\t")) != nullptr)
        {
            argc++;
        }

        // foreach input arg if env arg search and replace val
        for (int i = 0; i < argc; i++)
        {
            if (argv[i][0] == '$')
            {
                if (env.find(argv[i]) != env.end())
                {
                    argv[i] = const_cast<char *>(env.find(argv[i])->second.c_str());
                }
            }
        }

        __exec__(argv);
    }
    return 0;
};

void __exec__(char **argv)
{
    std::cout << "Hello World!" << std::endl;
};