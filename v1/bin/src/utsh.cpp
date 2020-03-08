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

void prompt()
{
    printf("\033[35m");
    printf("%s@%s$ ", SHELLNAME, getenv("USER"));
    printf("\033[0m");
}

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

        prompt();

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

        handleCmd(argv);
    }
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