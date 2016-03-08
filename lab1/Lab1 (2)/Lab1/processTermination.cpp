#include <iostream>
#include <unistd.h>
#include <signal.h>

/*
	Program written by James Calabro and Joe McCaffrey on Wed 9/23/15 for CS 377.

	Description of program from assignment webpage:
	Write a program that functions as a process termination utility (which we will refer to as the kill utility).
	Your program should prompt the user to enter a process id and then use the kill system call to terminate it.
	Be sure to handle error cases carefully, including errors where the user does not have privileges to terminate the specified process,
	or the user specifies an non-existent process.The return values from the kill system calls will indicate whether the call
	succeeded or what type of error occured.
*/

int main()
{
	// variables will hold the pid of the process to be terminated
	// and the results of the termination, respectively
	int terminatedPID = -1;
	int killOutcome = 0;

	// promt the user for their input
	std::cout << "Enter a process ID to be terminated: \n";
	std::cin >> terminatedPID;

	// try to kill the process and store the result in killOutcome
	try
	{
		killOutcome = kill(terminatedPID, 3);
	}
	catch (int exception)
	{
	}

	// if there was an error while performing kill, for example if there is no process with the
	// pid that was entered
	if (killOutcome != 0)
	{
		std::cout << "An error occured when attempting to kill process with pid " << terminatedPID << std::endl;
		return 1;
	}

	// sends a signal to check if the process with the entered pid has been closed and prints the result
	if (kill(terminatedPID, 0) == 0)
	{
		std::cout << "Unable to close process with id " << terminatedPID << ". It is possible that you do not have the necessary permissions.\n";
	}
	else
	{
		std::cout << "Process with id " << terminatedPID << " finished" << std::endl;
	}

	return 0;
}
