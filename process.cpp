#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <vector>
int total = 200;

int main(int argc,char **argv)
{
    pid_t  pid;
    int pipe1_2[2];
    pipe(pipe1_2);
    char* args[3];

    args[0] = (char*)"usr/bin/python";
    args[1] = (char*)"./test.py";
    args[2] = nullptr;

    pid = fork();

    if(pid == 0)
    {
        dup2(pipe1_2[1],STDOUT_FILENO);
        close(pipe1_2[0]);
        close(pipe1_2[1]);
        execv ("/usr/bin/python", args);
        std::cerr << "Error: an error occurred in execv" << std::endl;
    }

    else
    {
        dup2(pipe1_2[0],STDIN_FILENO);
        close(pipe1_2[0]);
        close(pipe1_2[1]);
        while(!std::cin.eof())
        {
            std::string line;
            std::getline(std::cin, line);
       	    std::cerr << "from cpp" << std::endl;
            std::cout << line << std::endl;
        }
    }

          
   return 0;
}
