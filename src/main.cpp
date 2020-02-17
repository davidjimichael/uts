// run with
// g++ main.cpp lib.h -o a.exe
// a.exe
#include "lib.h"

int main(int argc, char *argv[], char *arge[])
{
    // capture environment arguments
    std::vector<std::string> env_args;
    env_args.assign(arge, arge + lib::ENV_SIZE);

    // have user forfeit password
    std::string password;
    std::cout << "PROFILE: " << getenv("USERPROFILE") << '\n';
    std::cout << "PASSWORD: ";
    std::getline(std::cin, password);

    // create resource file
    std::string file_name = getenv("USERPROFILE") + lib::FAV_SEP + "Desktop" + lib::FAV_SEP + ".ourshrc";
    std::cout << "writing to resource file: " << file_name << std::endl;
    std::ofstream stream(file_name);

    // write executable path and password
    stream << lib::getexepath() << std::endl
           << password << std::endl;

    // write all environment arguments
    for (auto i = env_args.begin(); i != env_args.end(); ++i)
    {
        stream << *i << std::endl;
    }

    stream.close();

    return 0;
}