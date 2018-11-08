#include <iostream>
#include <sys/types.h>
#include <unistd.h>

int main ()
{
  pid_t child_pid;

  std::cout << "the main program process id is " << (int) getpid () << std::endl;
  child_pid = fork();
  if (child_pid != 0) {
	std::cout << "this is the parent process, with id " << (int) getpid () <<  std::endl;
    	std::cout << "the child's process id is " << (int) child_pid << std::endl;
  }
  else 
    	std::cout << "this is the child process, with id " << (int) getpid () << std::endl;

  return 0;
}


