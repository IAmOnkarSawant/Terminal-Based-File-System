#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <grp.h>
#include <ctime>
#include <cstdio>
#include <pwd.h>
#include <fcntl.h>

using namespace std;

string process_path(char *path)
{
    char *tokens = strtok(path, "/");
    string ans = "";
    while (tokens != NULL)
    {
        if (strcmp(tokens, "home") == 0)
        {
            tokens = strtok(NULL, "/");
            continue;
        }
        else if (strcmp(tokens, getlogin()) == 0)
        {
            tokens = strtok(NULL, "/");
            continue;
        }
        else
        {
            ans += "/";
            ans += tokens;
            ans += "/";
            tokens = strtok(NULL, "/");
        }
    }
    return ans;
}

void prompt()
{
    struct utsname system_info;
    if (uname(&system_info) == -1)
    {
        perror("uname");
        return;
    }

    // Prompt printing
    char currWorkingDirectory[1000];
    string Bash_prompt = "<";
    Bash_prompt += getlogin();
    Bash_prompt.push_back('@');
    Bash_prompt += system_info.sysname;
    Bash_prompt += ":~";
    Bash_prompt += process_path(getcwd(currWorkingDirectory, sizeof(currWorkingDirectory)));
    Bash_prompt += "> ";
    cout << Bash_prompt;
}