#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <string>
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


void echo(int num_of_args, char *args[])
{
    bool in_Quotes = 0;
    if(num_of_args == 1)
    {
        cout<<"\n";
        return;
    }
    string output;

    for (int i = 1; i < num_of_args; i++) 
    {
        string token = args[i];

        if (!in_Quotes && token[0] == '"') 
        {
            in_Quotes = true;
            if (token.size() > 1) 
            {
                token = token.substr(1);  
            } 
            else 
            {
                continue;  
            }
        }

        if (in_Quotes && token.back() == '"') 
        {
            in_Quotes = false;
            token.pop_back();  
        }

        if (in_Quotes) 
        {
            output += token + " ";
        }
        else 
        {
            cout << token << " ";
        }
    }
    // if (!output.empty() && (output_redirected || output_redirected_append)) 
    // {
        
    // }
    // else 
    // {
        cout << output<<"\n";
    // }
}