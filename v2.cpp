#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <map>
#include <cstring>
#include <string.h>
#include <string>
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
map<string, string> aliases;
vector<string> history;

int shcd(char **args);
int shhelp(char **args);
int shhistory(char **args);
int shaliases(char **args);
int shunalias(char **args);

// the keywords we will use for our builtin methods.
char *builtin_str[] = 
{
 (char*) "cd",
 (char*) "help",
 (char*) "history",
 (char*) "alias",
 (char*) "unalias",
};	

// array of funct pointers with string array input (builtin_str corresponding)
int (*builtin_func[]) (char**) = 
{
    &shcd,
    &shhelp,
    &shhistory,
    &shaliases,
    &shunalias,
};

// for reusability when adding builtins, calc bytes of arr divide by pointer size in system
int lsh_num_builtins() 
{
    return sizeof(builtin_str) / sizeof(char*);
};

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

int shaliases(char **args)
{
	// 0_alias 1_alias_name 2_alias_cmd
	
	if (args[1] != NULL && args[2] != NULL && args[3] != NULL)
	{
		// err too many input arguments
		std::cerr << "too many input args" << std::endl;
		return 1;
	}
	else if (args[1] == NULL)
	{
		// print all aliases
		for (auto iter = aliases.begin(); iter != aliases.end(); ++iter)
		{
			std::cout << "alias " << iter->first << " " << iter->second  << std::endl;
		}
	}
	else if (args[1] != NULL && args[2] == NULL)
	{
		// print specific alias
		std::cout << "alias " << args[1] << " " << aliases[args[1]] << std::endl;
	}
	else if (args[1] != NULL && args[2] != NULL)
	{
		// add alias to status
		aliases[args[1]]=args[2];
	}
	else
	{
		// just tell them it error and exit
		std::cerr << "bad aliases, try again" << std::endl;
		return 1;
	}

	return 0;
}

int shhistory(char **args)
{	
	int len;

	if (args[1] == NULL) 
	{
            len = history.size();
	}
        else
	{	
	try
	{
		int val = atoi(args[1]);
	
		if (0 <= val)
		{
			if (val < history.size())
			{
				len = val;
			}
		}
	       	else 
		{
			std::cerr << "cannot parse history of size " << args[1] << std::endl;
			return 1;
		}
	}
	catch (std::exception e)
	{
		return 1;
	}
	}
	int i;
	for (i = 0; i < len; i++) 
	{
	    try 
            {
		    std::cout << history.at(i) << std::endl;

//		for (string &cmd : history)
//		{
//		    std::cout << cmd << std::endl;
//		}
	    } 
	    catch (std::exception e)
	    {
		return 1;
	    }
	}

	return 0;
}

int shunalias(char** args)
{
	int erased = aliases.erase(args[1]);
	
	if (erased) {
		return 0;
	} else {
		std::cerr << "Cannot find alias " << args[1] << std::endl;
		return 1;
	}
}

void wrapup(void)
{
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


int init() 
{
//    if (login()) return 1;
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
	std::cout << "$ ";
	std::cout.flush();
        memset(line, '\0', MAXLINE);
        fgets(line, MAXLINE, stdin);
        
        if ((argv[0] = strtok(line, " \n\t")) == nullptr) 
	{
            continue;
        }
	history.push_back(argv[0]);
	if (strcmp(argv[0], "exit") == 0) 
	{
	    running = false;
	    _exit(1);
        }
        
        // input other args
        argc = 1;
        while ((argv[argc] = strtok(nullptr, " \n\t")) != nullptr) 
	{
	    history.back() += " ";
	    history.back() += argv[argc];
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
    wrapup();
}
