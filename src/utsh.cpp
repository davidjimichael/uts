/**
 * Mini-Shell/Our-Shell Project
 *
 * This Project is intended to demonstrate our
 * knowledge of C & C++ libraries & function which
 * contribute to how modern shells function.
 *
 * Author: Trenton Letz
 * Date: Feb 23, 2020
 */

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <map>
#include <string>
#include <vector>

#define SHELLNAME "mini-shell"
#define SECURE false
#define OURSHRC true
#define ALIASING true

using namespace std;

map<string, string> env;
map<string, vector<string>> aliases;

void shellPrompt();

int initShell();

int login();

int handleCmd(char **argv);

void readFile(char *file);

int main()
{
    char line[1024];
    char *argv[256];
    int argc;

    // Initialize some shell features.
    initShell();

    bool running = true;
    while (running)
    {

        shellPrompt();

        memset(line, '\0', 1024); // Clear the line

        fgets(line, 1024, stdin); // Read the input

        // If nothing is written, the loop is executed again.
        if ((argv[0] = strtok(line, " \n\t")) == nullptr)
            continue;

        // Test to exit.
        if (strcmp(argv[0], "exit") == 0)
        {
            running = false;
            continue;
        }

        // Read the rest of the arguments in.
        argc = 1;
        while ((argv[argc] = strtok(nullptr, " \n\t")) != nullptr)
            argc++;

        // Inject Environment Variables
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

        // Handle Aliasing
        if (ALIASING)
        {
            if (strcmp(argv[0], "alias") == 0)
            {
                vector<string> temp;
                if (argv[2] == nullptr)
                {
                    perror("Nothing to alias");
                }
                else
                {
                    for (int i = 2; i < argc; i++)
                    {
                        temp.emplace_back(argv[i]);
                    }
                    aliases.insert(pair<string, vector<string>>(argv[1], temp));
                }

                continue;
            }

            if (aliases.find(argv[0]) != aliases.end())
            {
                vector<string> found = aliases.find(argv[0])->second;
                argc = 0;

                for (size_t i = 0; i < found.size(); i++)
                {
                    argv[i] = const_cast<char *>(found[i].c_str());
                    argc++;
                }

                argv[argc] = nullptr;
            }
        }

        handleCmd(argv);
    }
}

void shellPrompt()
{
    printf("\033[35m");
    printf("%s@tletz$ ", SHELLNAME);
    printf("\033[0m");
}

int initShell()
{
    // Prompt login.
    if (SECURE)
    {
        int status = login();
        if (status != 0)
        {
            return EXIT_FAILURE;
        }
    }

    // Read in ourshrc file.
    if (OURSHRC)
    {
        readFile("../.ourshrc");
    }
}

int login()
{
    char user[64];
    char pass[64];

    // Get username
    printf("username: ");
    scanf("%s", &user);

    // Get password
    printf("password: ");
    scanf("%s", &pass);

    // Flush the stdin.
    fflush(stdin);

    // Check username & password
    if (strcmp(user, "tletz") == 0 && strcmp(pass, "password") == 0)
    {
        return 0;
    }

    return -1;
}

int handleCmd(char **argv)
{
    pid_t c_pid, pid;
    int status;

    c_pid = fork();

    if (c_pid == 0)
    {
        execvp(argv[0], argv);
        perror("command not found");

        // Flush the stdin.
        fflush(stdin);
    }
    else if (c_pid > 0)
    {
        if ((pid = wait(&status)) < 0)
        {
            perror("wait");
            _exit(1);
        }
    }
    else
    {
        perror("fork failed");
        _exit(1);
    }

    return 0;
}

void readFile(char *file)
{
    char line[1024];
    char *var[1024];

    FILE *ourshrc;
    ourshrc = fopen(file, "r");

    if (ourshrc != nullptr)
    {
        while (fgets(line, 1024, ourshrc))
        {
            var[0] = strtok(line, " \n\t");
            var[1] = strtok(nullptr, " \n\t");

            // Place Key & Value into Map
            env.insert(pair<string, string>(var[0], var[1]));
        }
    }
}