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
#include <vector>

using namespace std;

void cd(int num_of_args, char *args[], char currWorkingDirectory[], char preWorkingDirectory[])
{
    if(num_of_args == 1)
    {
        // because there can be multiple home
        if(getenv("HOME") != NULL)
        {
            int result = chdir(getenv("HOME"));
            if(result == 0)
                swap( currWorkingDirectory, preWorkingDirectory);                                                       
            else
                perror("cd");
        }
        else
        {
            cout<<"Home Directory path not found\n";
        }
        
    }
    else if(num_of_args == 2)
    {
        if(strcmp(args[1],"..") == 0)
        {
            int result = chdir(args[1]);
            if(result == 0)                                                     
                swap(preWorkingDirectory, currWorkingDirectory);
            else
                cout<<"Bash: "<<args[1]<<": Unable to execute command\n";
        }
        else if(strcmp(args[1],".") == 0)
        {
            cout<<"";
        }
        else if(strcmp(args[1],"-") == 0) 
        {
            int result = chdir(preWorkingDirectory);
            if(result == 0)
                swap(preWorkingDirectory, currWorkingDirectory);                                                      
            else
                cout<<"Bash: "<<args[1]<<": Unable to execute command\n";
        }
        else if(strcmp(args[1],"~") == 0)
        {
            if(getenv("HOME") != NULL)
            {
                int result = chdir(getenv("HOME"));
                if(result == 0)                                                       
                    swap(preWorkingDirectory, currWorkingDirectory);
                else
                    cout<<"Bash: "<<args[1]<<": Unable to execute command\n";
            }
            else
            {
                cout<<"Home Directory path not found\n";
            }
        }
        else if(strcmp(args[1],"/") == 0)
        {
            int result = chdir("/");
            if(result == 0)                                                       
                swap(preWorkingDirectory, currWorkingDirectory);
            else
                cout<<"Bash: "<<args[1]<<": Unable to execute command\n";
        }
        else
        {
            int result = chdir(args[1]);
            if(result == 0)
                swap(preWorkingDirectory, currWorkingDirectory);                                                     
            else
                cout<<"Bash: "<<args[1]<<": No such file or directory\n";
        }
    }
    else
    {
        cout<<"Bash:"<<args[0]<<": too many arguments\n";
    }
}
