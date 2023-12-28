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
using namespace std;

void redirection (char * args[], int input_path, int output_path, int output_index_a)
{
    cout<<input_path<<" "<<output_path<<" "<<output_index_a<<"\n";
    string input_file="", output_file="";
    if(input_path != -1)
    {
        input_file = args[input_path];
        //cout<<input_file<<" after\n";
    }
    if(output_path != -1)
        output_file = args[output_path];
    else if(output_index_a != -1)
        output_file = args[output_index_a];
    
    bool append_mode = 0;
    if(output_index_a != -1)
        append_mode = 1;

    cout<<"input file: "<<input_file<<"\noutput file: "<<output_file<<"\n";

    if (input_path==-1 && !input_file.empty())
    {
        cout<<"I am in\n";
        int input_fd = open(input_file.c_str(), O_RDONLY);
        if (input_fd == -1)
        {
            cerr << "Error opening input file: " << input_file << endl;
            return;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    //streambuf *coutbuf = cout.rdbuf(); 
    //ofstream output_stream;
    if ((output_path==-1) && (output_index_a == -1) && !output_file.empty())
    {
        int flags = O_WRONLY | O_CREAT;
        if (append_mode)
        {
            flags |= O_APPEND;
        }
        else
        {
            flags |= O_TRUNC;
        }

        int output_fd = open(output_file.c_str(), flags, 0644);
        if (output_fd == -1)
        {
            cerr << "Error opening output file: " << output_file << endl;
            return;
        }

        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

}