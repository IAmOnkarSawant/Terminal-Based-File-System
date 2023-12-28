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

void pwd()
{
    char pwd_path[1000];
    getcwd(pwd_path, sizeof(pwd_path));
    cout<<pwd_path<<"\n";
}