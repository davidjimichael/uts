// set_env_demo
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
// static map
int main()
{
    // loop
    char chProgramVar[] = "hello world";
    _putenv_s("ShellVar", chProgramVar);

    system("echo $ShellVar");
}