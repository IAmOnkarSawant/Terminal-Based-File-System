#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <grp.h>
#include <ctime>
#include <cstdio>
#include <pwd.h>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;


void process_info(int num_of_args, char *args[])
{
    if (num_of_args == 1)
    {
        int pid = getpid();
        cout << "Process ID: " << pid << endl;

        stringstream status_path;
        status_path << "/proc/" << pid << "/stat";

        ifstream status_file(status_path.str());
        if (status_file.is_open())
        {
            string status_line;
            getline(status_file, status_line);
            status_file.close();

            stringstream ss(status_line);
            string token;
            int token_count = 1;
            while (getline(ss, token, ' '))
            {
                if (token_count == 3)
                {
                    cout << "Status: " << token << endl;
                }
                else if (token_count == 23)
                {
                    cout << "Memory Usage: " << stol(token) / 1024 << " KB" << endl;
                }
                token_count++;
            }
        }

        stringstream exe_path;
        exe_path << "/proc/" << pid << "/exe";
        char executable_path[5000];
        ssize_t path_length = readlink(exe_path.str().c_str(), executable_path, sizeof(executable_path));
        if (path_length != -1)
        {
            executable_path[path_length] = '\0';
            cout << "Executable Path: " << executable_path << endl;
        }
        else
        {
            cout << "Error reading executable path: " << strerror(errno) << endl;
        }
    }
    else if (num_of_args == 2)
    {
        int pid = atoi(args[1]);
        if (kill(pid, 0) == 0)
        {
            cout << "Process ID: " << pid << endl;
            stringstream status_path;
            status_path << "/proc/" << pid << "/stat";

            ifstream status_file(status_path.str());
            if (status_file.is_open())
            {
                string status_line;
                getline(status_file, status_line);
                status_file.close();

                stringstream ss(status_line);
                string token;
                int token_count = 1;
                while (getline(ss, token, ' '))
                {
                    if (token_count == 3)
                    {
                        cout << "Status: " << token << endl;
                    }
                    else if (token_count == 23)
                    {
                        cout << "Memory Usage: " << stol(token) / 1024 << " KB" << endl;
                    }
                    token_count++;
                }
            }

            stringstream exe_path;
            exe_path << "/proc/" << pid << "/exe";
            char executable_path[5000];
            ssize_t path_length = readlink(exe_path.str().c_str(), executable_path, sizeof(executable_path));
            if (path_length != -1)
            {
                executable_path[path_length] = '\0';
                cout << "Executable Path: " << executable_path << endl;
            }
            else
            {
                cout << "Error reading executable path: " << strerror(errno) << endl;
            }
        }
        else
        {
            cout << "Invalid process ID: " << pid << endl;
        }


    }
    else
    {
        cerr << "Usage: pinfo [PID]" << endl;
    }
}


