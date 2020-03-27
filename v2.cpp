#include<bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <map>
#include <cstring>
#include <string.h>
#include <vector>

// simplicity and typing
using std::map;
using std::vector;
using std::string;
using std::pair;

#define MAXLINE 1024
#define MAXCMD 256

// easiest way to have env data
map<string, string> env;
vector<string> history;

int shcd(char **args);
int shhelp(char **args);
int shhistory(char **args);

int rfhistory(char **args);
int rfconfig(char **args);

// the keywords we will use for our builtin methods.
char *builtin_str[] = 
{
    "cd",
    "help",
    "history",
};	

char *rff_str[] =
{
    "config",
    "history",
};

// array of funct pointers with string array input (builtin_str corresponding)
int (*builtin_func[]) (char**) = 
{
    &shcd,
    &shhelp,
    &shhistory,
};

int (*read_file_funct[]) (char**) =
{
    &rfhistory,
    &rfconfig,
};

// for reusability when adding builtins, calc bytes of arr divide by pointer size in system
int lsh_num_builtins() 
{
    return sizeof(builtin_str) / sizeof(char*);
};

int len_rff()
{
    return sizeof(rff_str) / sizeof(char*);
}

int shcd(char **args)
{
    if (args[1] == NULL) 
    {
        chdir(std::getenv("HOME"));
    } 
    else 
    {
        if (chdir(args[1]) != 0) 
        {
       	    return 1; // error status for a bad cd handle later todo
	}
    }
    
    return 0;
}

int shhelp(char **args)
{
    int i;
 
    printf("built-ins:\n");

    for (i = 0; i < lsh_num_builtins(); i++) 
    {
        printf("  %s\n", builtin_str[i]);
    }

    return 0;
}

int shhistory(char **args)
{	
	int i;
	int len;

	if (args[1] == NULL) 
	{
            len = history.size();
	}
        
	try
	{
		int val = atoi(args[i]);
	
		if (isdigit(val) && 0 <= val && val <= history.size()) 
		{
			len = val;
		}
	       	else 
		{
			std::cerr << "Cannot parse history of size " << args[i] << std::endl;
			return 1;
		}
	}
	catch (std::exception e)
	{
		return 1;
	}
	
	for (i = 0; i < len; i++) 
	{
	    try 
            {
		std::cout << history.at(i) << std::endl;
	    } 
	    catch (std::exception e)
	    {
		return 1;
	    }
	}

	return 0;
}

int login() 
{
    char user[64];
    char pass[64];
    printf("username (='user'): ");
    scanf("%s64", user);
    printf("password (='pass'): ");
    scanf("%s64", pass);
    fflush(stdin);
    
    if (strcmp(user, "user") == 0 && strcmp(pass, "pass") == 0) 
    {
        return 0;
    }

    return -1;
}

int rfconfig(char **args) 
{
    char *var[64];

    for (unsigned int i=0; args[i] != NULL; i++) 
    {
        try
        {
            var[0] = strtok(args[i], " \n\t");
            var[1] = strtok(nullptr, " \n\t");
            env.insert(pair<string, string>(var[0], var[1]));
        }
        catch (std::exception e)
        {
            return 1;
        }
    }

    return 0;
}

int rfhistory(char **args)
{
    for (unsigned int i=0; args[i] != NULL; i++) {
        history.push_back(args[i]);
    }
}

int readFile(char *file) 
{
    char line[1024];
    char *lines[2048];
    int i;

    FILE *ourshrc;
    if ((ourshrc = fopen(file, "r")) != nullptr) 
    {
        while (fgets(line, 1024, ourshrc)) 
	{
            lines[i++] = line;
        }
    } else {
	    std::cerr << "unable to read file " << file << std::endl;
        return 1;
    }

    int j;
    for (j = 0; j < i; j++) {
        if (strcmp(file, rff_str[j]) == 0) {
            try
            {
                (*read_file_funct[j])(lines);
            }
            catch (std::exception e)
            {
                return 1;
            }
        }
    }
}

int init() 
{
//    if (login()) return 1;
    if (readFile(".ourshrc") != 0) 
    {
        std::cerr << "unable to read .ourshrc" << std::endl;
    }
    if (readFile(".history") != 0)
    {
        std::cerr << "unable to read .history" << std::endl;
    }

    return 0;
}

int cmdexec(char **argv) 
{
    pid_t c_pid, pid;
    int status;

    c_pid = fork();

    if (c_pid == 0) {
        if (execvp(argv[0], argv) == -1)
		{
 			perror("bad cmd");
		}
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
        perror("bad fork");
        //_exit(1);
    }

    return 0;
}

void loop(void)
{
    char line[MAXLINE];
    char *argv[MAXCMD];
    int argc;
    bool running = true;


    while (running)
    {
        // prompt user, get input, clear and continue if empty
	    printf("$ ");
        memset(line, '\0', 1024);
        fgets(line, 1024, stdin);
        
        if ((argv[0] = strtok(line, " \n\t")) == nullptr) 
		{
            continue;
        }

	    if (strcmp(argv[0], "exit") == 0) 
		{
		    running = false;
		    continue;
	    }
        
        // input other args
        argc = 1;
        while ((argv[argc] = strtok(nullptr, " \n\t")) != nullptr) 
		{
            argc++;
        }

        // input env
        for (int i = 0; i < argc; i++) 
		{
            if (argv[i][0] == '$') {
                if (env.find(argv[i]) != env.end()) 
				{
                    // append to arguments for passing to handle
                    argv[i] = const_cast<char *>(env.find(argv[i])->second.c_str());
		            std::cout << "Set ENV: " << argv[i] << std::endl;
                }
            }
        }

	    history.push_back(line);
        int e = 1;
        for (int i = 0; i < lsh_num_builtins(); i++) 
		{
            if (strcmp(argv[0], builtin_str[i]) == 0) 
			{
                e = 0;
                (*builtin_func[i])(argv);
            }
        }
        if (e) cmdexec(argv);
    }
}

int main()
{
    init();
    loop();
}

