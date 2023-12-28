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

void ls_print(string path, bool is_hidden, bool is_all)
{
    struct dirent* ls_items;
    DIR* directory = opendir(path.c_str());
    if (directory == NULL) 
    {
        perror("opendir");
        return;
    }

    //iterating direcory
    while(1)
    {
        ls_items = readdir(directory);
        if(ls_items == NULL)
        {
            break;
        }
        else 
        {
            if((is_all) && ((ls_items->d_name[0] == '.' && is_hidden) || (ls_items->d_name[0] != '.')))
            {
                struct stat file_info;
                string list_content = "";
                string item_l (ls_items->d_name);
                string new_path = path + "/" + item_l;
                int err = stat(new_path.c_str(), &file_info);
                if(err == -1)
                    perror("stat");
                
                string permission = "";
                permission += (char)(S_ISDIR(file_info.st_mode)) ? "d" : "-";
                permission += (char)(file_info.st_mode & S_IRUSR) ? "r" : "-";
                permission += (char)(file_info.st_mode & S_IWUSR) ? "w" : "-";
                permission += (char)(file_info.st_mode & S_IXUSR) ? "x" : "-";
                permission += (char)(file_info.st_mode & S_IRGRP) ? "r" : "-";
                permission += (char)(file_info.st_mode & S_IWGRP) ? "w" : "-";
                permission += (char)(file_info.st_mode & S_IXGRP) ? "x" : "-";
                permission += (char)(file_info.st_mode & S_IROTH) ? "r" : "-";
                permission += (char)(file_info.st_mode & S_IWOTH) ? "w" : "-";
                permission += (char)(file_info.st_mode & S_IXOTH) ? "x" : "-";
                
                //for permissions
                cout<<permission<<" ";
                
                // for links
                long long link = (long long)(file_info.st_nlink);
                string stlink = to_string(link);
                while(stlink.size() < 3)
                {
                    string space = " ";
                    stlink = space + stlink;
                }
                cout<<stlink<<" ";
                
                // for owner and group name
                cout<<getpwuid(file_info.st_uid)->pw_name<<" ";
                cout<<getgrgid(file_info.st_gid)->gr_name<<" ";
                
                
                long long fsize = (long long)(file_info.st_size);
                string file_size = to_string(fsize);
                while(file_size.size() < 6)
                {
                    string space = " ";
                    file_size = space + file_size;
                }
                cout<<file_size<<" ";
                
                string time_in_Str = ctime(&file_info.st_mtime);
                time_in_Str.pop_back(); 
                time_in_Str = time_in_Str.substr(4, 12);
                cout<<time_in_Str.c_str()<<" ";
            
            }   
            if((ls_items->d_name[0] == '.' && is_hidden) || (ls_items->d_name[0] != '.'))
            {
                cout<<ls_items->d_name<<"   ";
            }
        }
    
        if((is_all) && ((ls_items->d_name[0] == '.' && is_hidden) || (ls_items->d_name[0] != '.')))
        {
            cout<<"\n";
        }
            
    }
     if(!is_all)
        cout<<"\n";
}

void ls(int num_of_args, char *args[])
{
    if(num_of_args == 1)
    {
        ls_print("./", 0, 0);
    }
    else if((num_of_args == 2) || (num_of_args == 3)||(num_of_args == 4))
    {
        if(((num_of_args==4) && (strcmp(args[1], "-l")==0) && (strcmp(args[2], "-a")==0)) || ((num_of_args==3) && (strcmp(args[1], "-l")==0 && (strcmp(args[2], "-a")==0))) ) 
        {
            if(num_of_args == 4 ) 
            {
                ls_print(args[3], 1, 1);
            }
            else
                ls_print("./", 1, 1);
        }
        else if(strcmp(args[1], "-l") == 0)
        {
            if(num_of_args == 3)
                ls_print(args[2], 0, 1);
            else
                ls_print("./", 0, 1);
        }
        else if(strcmp(args[1], "-a") == 0)
        {
            if(num_of_args == 3)
                ls_print(args[2], 1, 0);
            else
                ls_print("./", 1, 0);
        }
        else if(strcmp(args[1], ".") == 0)
        {
            if(num_of_args == 3)
                ls_print(args[2], 0, 0);
            else
                ls_print("./", 0, 0);
        }
        else if(strcmp(args[1], "..") == 0)
        {
            if(num_of_args == 3)
                ls_print(args[2], 0, 0);
            else
                ls_print("..", 0, 0);
        }
        else if(strcmp(args[1], "~") == 0)
        {
            if(getenv("HOME") != NULL)
                ls_print(getenv("HOME"), 0, 0);
        }
        else if((strcmp(args[1], "-la") == 0) || (strcmp(args[1], "-al") == 0))
        {
            if(num_of_args == 3)
                ls_print(args[2], 1, 1);
            else
                ls_print("./", 1, 1);
        }
    }
    else
    {
        cout<<"Bash:"<<args[0]<<": too many arguments\n";
        return;
    }
}