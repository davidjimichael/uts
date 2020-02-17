#include "lib.h"

int main(int argc, char *argv[], char *arge[])
{
    // capture environment arguments
    std::vector<std::string> env_args;
    env_args.assign(arge, arge + lib::ENV_SIZE);

    // have user forfeit password
    char password[256];
    std::cout << "PROFILE: " << getenv("USERPROFILE") << '\n';
    std::cout << "PASSWORD: ";
    std::scanf("%d", &password);
    getchar();

    // create resource file
    std::fstream stream(getenv("USERPROFILE") + lib::FAV_SEP + "Desktop" + lib::FAV_SEP + ".ourshrc");

    // write executable path
    stream << lib::getexepath() << std::endl;

    // write user password
    stream << password << std::endl;

    // write all environment arguments
    for (auto i = env_args.begin(); i != env_args.end(); ++i)
    {
        stream << *i << std::endl;
    }

    stream.close();

    return 0;
}