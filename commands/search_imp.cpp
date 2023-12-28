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

bool search(const char* directory_path, const char* search_name) 
{
    struct dirent* ls_items;
    DIR* directory = opendir(directory_path);
    if (directory == NULL) 
    {
        return false;
    }

    while (1) 
    {
        string item_path = directory_path;
        ls_items = readdir(directory);
        if(ls_items == NULL)
        {
            break;
        }
        else 
        {
            if (strcmp(ls_items->d_name, ".") == 0 || strcmp(ls_items->d_name, "..") == 0) 
                continue;//hidden
        }

        item_path += "/";
        item_path += ls_items->d_name;
        struct stat file_info;
        int err = stat(item_path.c_str(), &file_info);
        if ( err == -1) 
        {
            perror("stat");
            return false;
        }

        if (S_ISDIR(file_info.st_mode)) 
        {
            // cout<<item_path<<" "<<search_name<<"\n";
            if (search(item_path.c_str(), search_name)) 
            {
                closedir(directory);
                return true;
            }
        } 
        else 
        {
            // cout<<item_path<<"\n";
            if (strcmp(ls_items->d_name, search_name) == 0) 
            {
                closedir(directory);
                return true;
            }
        }
    }
    closedir(directory);
    return false;
}

void search_imp(int num_of_args, char* args[])
{
    if (search("./", args[1])) 
        cout << "True\n" ;
    else 
        cout << "False\n";
}