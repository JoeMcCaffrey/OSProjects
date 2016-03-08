#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

/*
	Program written by James Calabro and Joe McCaffrey on Wed 9/23/15 for CS 377.

	Description of program from assignment webpage:
	This component of the lab requires you to use the fork system call. Write a program using fork() where a parent process creates a child process and a
	child process creates a grand-child. The grand-child process sleeps for 10 seconds and exits after printing "Grandchild process with process id [PID] has completed."
	The child process should wait for the grand-child to finish and once it does so, it should exit after printing "Child process with process ID [PID] has completed."
	The parent process should wait for the child process to finish and then exit after printing "Parent process with process ID [PID] has finished.
*/

int main()
{
	// status pointers to allow us to call fork()
	int *firstStatus;
	int *secondStatus;
	int time = 10;

	// fork the first time
	pid_t firstPID = fork();
	if (firstPID == 0)
	{
		// fork the second time
		pid_t secondPID = fork();
		if (secondPID == 0)
		{
			// sleep for 10 seconds
			sleep(10);
			std::cout << "Grandchild process with process id " << getpid() << " has completed after " << time << " seconds.\n";
		}
		else
		{
			// wait for the grandchild to finish
			waitpid(secondPID, secondStatus, 0);
			std::cout << "Child process with process id " << getpid() << " has completed after " << time << " seconds.\n";
		}
	}
	else
	{
		// wait for the child to finish
		waitpid(firstPID, firstStatus, 0);
		std::cout << "Process with process id " << getpid() << " has completed after " << time << " seconds.\n";
	}

	return 0;
}
