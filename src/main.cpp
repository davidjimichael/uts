#include "lib.h"

int main(int argc, char *argv[], char *arge[])
{
    std::vector<std::string> env_args;
    env_args.assign(arge, arge + lib::ENV_SIZE);

    for (auto i = env_args.begin(); i != env_args.end(); ++i)
        std::cout << *i << ' ';

    std::cout << "\nexe path: " << lib::getexepath() << std::endl;

    return 0;
}