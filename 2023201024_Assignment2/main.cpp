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
#include <signal.h>
#include "ls_imp.cpp"
#include "echo_imp.cpp"
#include "cd_imp.cpp"
#include "pwd_imp.cpp"
#include "search_imp.cpp"
#include "input_output_red.cpp"
#include "prompt.cpp"
#include "pinfo_imp.cpp"
#include "autocomplete.cpp"

using namespace std;

int foreground_pid = -1;
vector<string>history;

void handle_SIGTSTP(int signo) 
{
    if (foreground_pid != -1) 
    {
        kill(foreground_pid, SIGTSTP);
    }
}

void handle_SIGINT(int signo) 
{
    if (foreground_pid != -1) 
    {
        kill(foreground_pid, SIGINT);
    }
}

int main()
{
    // to check is background process 
    vector<int> background_pids;
    signal(SIGTSTP, handle_SIGTSTP);
    signal(SIGINT, handle_SIGINT);

    // for cd - command
    char preWorkingDirectory[1000];
    char currWorkingDirectory[1000];
    while (true)
    {
        label1:
        prompt();

        bool is_background = 0;
        bool input_redirected = 0;
        bool output_redirected = 0;
        bool output_redirected_append = 0;
        int  input_redirection_index = -1;
        int  output_redirection_index = -1;
        int  output_redirection_index_a = -1;
        char currWorkingDirectory[1000];
        
        getcwd(currWorkingDirectory, sizeof(currWorkingDirectory));
        
        // terminal input
        char Bash_input[1000], Bash_input1[1000];
        cin.getline(Bash_input, 1000);
        
        //^D
        if (cin.eof()) 
        {
            cout << "\n See you soon!" << endl;
            break; 
        }
        string input (Bash_input);
        if (input.find("\t") != string::npos) 
        {
            bool list = tabCompletion(input);
            if(list)
            {
                continue;
            }
            else
                strcpy(Bash_input ,input.c_str());
        } 
        else if (input == "exit") 
            break;
        

        strcpy(Bash_input1, Bash_input);

        // tokanize
        char *token = strtok(Bash_input, " \n\t");

        if (token != NULL)
        {

            if (strcmp(token, "exit") == 0)
            {
                cout << "exit\n";
                break;
            }
            else
            {
                int num_of_args = 0;
                char *args[1000];
                while (token != NULL)
                {
                    //***********************************************************
                    //    Checking for redirection any special symbol
                    //***********************************************************
                    
                    if (strcmp(token, ">") == 0)
                    {
                        // Check for output redirection and the presence of an output file
                        token = strtok(NULL, " \n\t");
                        if (token != NULL)
                        {
                            output_redirection_index = num_of_args + 1;
                            output_redirected = true;
                            args[num_of_args++] = token;
                        }
                        else
                        {
                            cout << "Error: Missing output file after '>'" << endl;
                            break;
                        }
                    }
                    else if (strcmp(token, ">>") == 0)
                    {
                        // Check for append output redirection and the presence of an output file
                        token = strtok(NULL, " \n\t");
                        if (token != NULL)
                        {
                            output_redirection_index_a = num_of_args + 1;
                            output_redirected_append = true;
                            args[num_of_args++] = token;
                        }
                        else
                        {
                            cout << "Error: Missing output file after '>>'" << endl;
                            break;
                        }
                    }
                    else if (strcmp(token, "<") == 0)
                    {
                        // Check for input redirection and the presence of an input file
                        token = strtok(NULL, " \n\t");
                        if (token != NULL)
                        {
                            input_redirection_index = num_of_args + 1;
                            input_redirected = true;
                            args[num_of_args++] = token;
                        }
                        else
                        {
                            cout << "Error: Missing input file after '<'" << endl;
                            break;
                        }
                    }
                    else
                    {
                        args[num_of_args++] = token;
                        token = strtok(NULL, " \n\t");
                    }
                }
                // cout<<"\n";
                args[num_of_args] = NULL;

                if (strcmp(args[num_of_args - 1], "&") == 0)
                {
                    is_background = 1;
                    args[num_of_args - 1] = NULL;
                    num_of_args--;
                }

                // CD
                if (strcmp(args[0], "cd") == 0)
                {
                    cd(num_of_args, args, currWorkingDirectory, preWorkingDirectory);
                }

                // ECHO
                else if (strcmp(args[0], "echo") == 0)
                {
                    int saved_stdout = dup(STDOUT_FILENO);
                    if (output_redirected || output_redirected_append)
                    {
                        
                        int output_fd;
                        if (output_redirected_append)
                        {
                            output_fd = open(args[output_redirection_index_a], O_WRONLY | O_APPEND | O_CREAT, 0644);
                        }
                        else
                        {
                            output_fd = open(args[output_redirection_index], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        }

                        if (output_fd == -1)
                        {
                            perror("open");
                        }
                        else
                        {
                            dup2(output_fd, STDOUT_FILENO);
                            close(output_fd);
                        }
                        output_redirected = false;
                    }
                    echo(num_of_args, args);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdout);
                    continue;
                }

                // PWD
                else if (strcmp(args[0], "pwd") == 0)
                {
                    pwd();
                }

                //LS
                else if (strcmp(args[0], "ls") == 0)
                {
                    int saved_stdin = dup(STDIN_FILENO);
                    int saved_stdout = dup(STDOUT_FILENO);
                    if (input_redirected)
                    {
                        int input_fd = open(args[input_redirection_index], O_RDONLY);
                        if (input_fd == -1)
                        {
                            perror("open");
                        }
                        else
                        {
                            dup2(input_fd, STDIN_FILENO);
                            close(input_fd);
                        }
                        input_redirected = false;
                    }

                    if (output_redirected || output_redirected_append)
                    {
                        int output_fd;
                        if (output_redirected_append)
                        {
                            output_fd = open(args[output_redirection_index_a], O_WRONLY | O_APPEND | O_CREAT, 0644);
                        }
                        else
                        {
                            output_fd = open(args[output_redirection_index], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        }

                        if (output_fd == -1)
                        {
                            perror("open");
                        }
                        else
                        {
                            dup2(output_fd, STDOUT_FILENO);
                            close(output_fd);
                        }
                        output_redirected = false;
                    }
                    ls(num_of_args, args);

                    dup2(saved_stdin, STDIN_FILENO);
                    close(saved_stdin);

                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdout);
                    
                    continue;
                }

                // Pinfo
                else if (strcmp(args[0], "pinfo") == 0)
                {
                    // cout<<"in search\n";
                    process_info(num_of_args, args);
                }

                // SEARCH
                else if (strcmp(args[0], "search") == 0)
                {
                    // cout<<"in search\n";
                    search_imp(num_of_args, args);
                }

                // OTHER
                else
                {
                    //cout<<"in fork\n";
                    int pid = fork();
                    // cout<<pid<<" :PID\n";
                    if (pid < 0)
                        perror("fork");

                    else if (pid == 0)
                    {
                        // cout<<"Parent: ";
                        // cout<<input_redirected<<" - "<<output_redirected<<"\n";
                        
                        if ( input_redirected)
                        {   

                            // cout<<"\n"<< args[input_redirection_index]<<"IRI \n";
                            string curr = "./";
                            string re_path = curr + args[input_redirection_index];
                            int input_fd = open(args[input_redirection_index], O_RDONLY);
                            if (input_fd == -1)
                            {
                                cout << "Error opening input file: " << args[input_redirection_index] << endl;
                                exit(1);
                            }
                            dup2(input_fd, STDIN_FILENO);
                            close(input_fd);
                            
                            input_redirected = 0;
                        }

                        if ( (output_redirected || output_redirected_append))
                        {
                            //cout<<args[output_redirection_index]<<" ORI \n";
                            int output_fd;
                            string curr = "./";
                            string re_path = curr + args[output_redirection_index];
                            if (output_redirected_append)
                            {
                                output_fd = open(args[output_redirection_index_a], O_WRONLY | O_APPEND | O_CREAT, 0644);
                            }
                            else
                            {
                                output_fd = open(args[output_redirection_index], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                            }

                            if (output_fd == -1)
                            {
                                perror("open");
                                exit(1);
                            }
                            else
                            
                            dup2(output_fd, STDOUT_FILENO);
                            close(output_fd);
                            output_redirected = 0;
                        }
                        
                        int res = execvp(args[0], args);
                        if(res == -1)
                        {
                            perror("execvp");
                            exit(1);
                        }
                        
                    }
                    else 
                    {
                        //cout<<"CHILD: ";

                        foreground_pid = pid;
                        if (is_background)
                        {
                            background_pids.push_back(pid);
                            cout << "[" << background_pids.size() << "] " << pid << "\n";
                        }
                        else
                        {
                            int x;
                            waitpid(pid, &x, 0);
                            foreground_pid = -1;
                        }

                        for (int i = 0; i < (int)background_pids.size(); i++)
                        {
                            int bg_pid = background_pids[i];
                            int status;
                            if (waitpid(bg_pid, &status, WNOHANG) == bg_pid)
                            {
                                cout << "[" << background_pids.size() << "]+        " << bg_pid << "\n";
                                background_pids.erase(background_pids.begin() + i);
                                // background_pids.erase(background_pids.begin() + i);
                                i--;
                            }
                        }
                    }
                    continue;
                }
            }
        }
        for (int i = 0; i < (int)background_pids.size(); i++)
        {
            int bg_pid = background_pids[i];
            int status;
            if (waitpid(bg_pid, &status, WNOHANG) == bg_pid)
            {
                cout << "[" << background_pids.size() << "]+  DONE      " << bg_pid << "\n";
                background_pids.erase(background_pids.begin() + i);
                i--;
            }
        }
    }
    return 0;
}
