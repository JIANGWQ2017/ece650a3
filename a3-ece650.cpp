// based on the example from
// https://stackoverflow.com/questions/13041416/redirect-stdout-of-two-processes-to-another-processs-stdin-in-linux-c
#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;
/// Entry point of process A
int A1(void) {
	char* argv[3];
	argv[1] = (char*)"./a1ece650.py";
	argv[0]= (char*)"/usr/bin/python";
	argv[2] = nullptr;
	execv("/usr/bin/python",argv);
	std::cout << "Did not call python scripyt"<<std::endl;
	return 0;
}

int A2(void) {
	char* argv[2];
	argv[0] = (char*)"./a2";
	argv[1] = nullptr;
	execv("./a2",argv);
	std::cout << "Did not call a2-ece650.cpp"<<std::endl;
	return 0;
}


int procUserInput(void)
{
	while (!std::cin.eof()) {
	// read a line of input until EOL and store in a string
	std::string line;
	std::getline(std::cin, line);
	if (line.size () > 0)
		std::cout <<line<<endl;
		std::cout<<flush;
	}
	std::cout <<"\n"<< std::endl;
	return 0;
}




int main(int argc, char **argv)
{
	std::vector<pid_t> kids;
	// create a pipe
	int ABtoC[2];
	int RtoP[2];
	pipe(RtoP);
	pipe(ABtoC);


////////// child process 1//////////////
	pid_t child_pid;
	child_pid = fork ();
	if (child_pid == 0) // child process run regen
	{
		// redirect stdout to the pipe
		dup2(RtoP[1], STDOUT_FILENO);
		close(RtoP[1]);
		close(RtoP[0]);
		char* argv1[argc+1];
		argv1[0] = (char*)"./rgen";
		for(int i=1; i<argc; i++)
		{
			argv1[i] = argv[i];
		}
		argv1[argc] = NULL;
		execv("./rgen",argv1);
		std::cout << "Did not call a2-ece650.cpp"<<std::endl;
		return 0;
	}
	
	else if (child_pid < 0) // fork function doesnot work
	{
		std::cerr << "Error: could not fork\n";
		return 1;
	}
	
	kids.push_back(child_pid);
	

/////////////////Child process 2 ////////////////
	child_pid = fork ();
	if (child_pid == 0) // child process run A1
	{
		// redirect stdout to the pipe
		dup2(RtoP[0], STDIN_FILENO);
		close(RtoP[1]);
		close(RtoP[0]);
		
		dup2(ABtoC[1], STDOUT_FILENO);
		close(ABtoC[1]);
		close(ABtoC[0]);
	// start PYthon scripyt

		return A1();
	}
	else if (child_pid < 0) // fork function doesnot work
	{
		std::cerr << "Error: could not fork\n";
		return 1;
	}
	kids.push_back(child_pid);
	
	
	
	
///////////// child process 3/////////////////
	child_pid = fork ();
	if (child_pid == 0) // child process run A2
	{
		dup2(ABtoC[0], STDIN_FILENO);
		close(ABtoC[1]);
		close(ABtoC[0]);
		return A2();
	}
	else if (child_pid < 0) // fork function doesnot work
	{
		std::cerr << "Error: could not fork\n";
		return 1;
	}
	
	
	kids.push_back(child_pid);
//////// main process (user input)//////////
	dup2(ABtoC[1], STDOUT_FILENO);
	close(ABtoC[1]);
	close(ABtoC[0]);
	int res = procUserInput();

	// send kill signal to all children
	for (pid_t k : kids) {
		int status;
		kill (k, SIGTERM);
		waitpid(k, &status, 0);
	}

	return res;
}
