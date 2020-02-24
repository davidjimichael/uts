#include "lib.h"

int main(int argc, char **argv, char **arge)
{
    std::vector<std::string> env_args;
    for (char **env = arge; *env != 0; env++)
    {
        char *thisEnv = *env;
        env_args.push_back(thisEnv);
    }

    // have user forfeit password
    std::string password;
    std::cout << "PROFILE: " << getenv("USERPROFILE") << '\n';
    std::cout << "PASSWORD: ";
    std::getline(std::cin, password);

    std::string file_name = getenv("USERPROFILE") + lib::FAV_SEP + "Desktop" + lib::FAV_SEP + ".ourshrc";

    std::string line;
    std::ifstream fin(file_name);

    if (fin.is_open())
    {
        while (getline(fin, line))
        {
            std::cout << line << '\n';
        }
        fin.close();
    }

    return 0;
}