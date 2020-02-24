#include "lib.h"

int main(int argc, char **argv, char **arge)
{
    std::vector<std::string> env_args;
    for (char **env = arge; *env != 0; env++)
    {
        char *thisEnv = *env;
        env_args.push_back(thisEnv);
    }

    // Get username and password
    std::string username;
    std::string password;
    std::cout << "PROFILE: ";
    std::getline(std::cin, username);
    std::cout << "PASSWORD: ";
    std::getline(std::cin, password);
    std::cout << username << " " << password << std::endl;

    std::string file_name = getenv("USERPROFILE") + lib::FAV_SEP + "Desktop" + lib::FAV_SEP + ".ourshrc";
    std::cout << file_name << std::endl;

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