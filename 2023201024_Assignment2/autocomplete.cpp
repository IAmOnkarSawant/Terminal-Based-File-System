#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <grp.h>
#include <ctime>
#include <cstdio>
#include <pwd.h>
#include <fcntl.h>
#include <ncurses.h>


using namespace std;

vector<string> splitString(char Bash_input[]) 
{
    vector<string> tokens;
    int num_of_args = 0;
    char *args[1000];
    char *token = strtok(Bash_input, " \n\t");
    while (token != NULL)
    {
        string temp(token);
        tokens.push_back(temp);
        args[num_of_args++] = token;
        token = strtok(NULL, " \n\t");
    }
    args[num_of_args] = NULL;
    return tokens;
}

string getPartialCommand(string input) 
{
    char Bash_input[1000];
    strcpy(Bash_input, input.c_str());
    vector<string> tokens = splitString(Bash_input);
    if (!tokens.empty()) 
    {
        return tokens.back();
    }
    return "";
}

void get_all_file_folders(vector<string>&possibilities)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            possibilities.push_back(ent->d_name);
        }
        closedir(dir);
    }
}

bool tabCompletion(string &input)
{
    vector<string> matches ;
    vector<string> possibilities = {"ls", "cd", "echo", "pwd", "pinfo", "search", "exit"};
    get_all_file_folders(possibilities);

    string partial_command = getPartialCommand(input);

    for (const string &possibility : possibilities) 
    {
        if (possibility.find(partial_command) == 0) 
        {
            matches.push_back(possibility);
        }
    }

    if(matches.size() == 0)
        return 1;
    else if(matches.size() == 1)
    {

        return 0;
    }
    else
    {
        for (const string &match : matches) 
                cout<<match<<"   ";
        cout<<"\n";
        return 1;
    }
}
