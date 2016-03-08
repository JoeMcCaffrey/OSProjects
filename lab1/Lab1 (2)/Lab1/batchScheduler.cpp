#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iterator>
#include <sys/wait.h>


/*
	Program written by James Calabro and Joe McCaffrey on Wed 9/23/15 for CS 377.

	Description of program from assignment webpage:
	The last component of the lab involves writing a batch scheduling shell. This shell program takes as input a sequence of batch programs to run.
	The shell then runs these programs sequentially one after another. This is done by forking a child process and having it
	 use exec to run the specified program. The parent shell process waits for the child to finish and then runs the next program in a same fashion.
	Your batch program should be able to take an arbitrary list of programs from the command line and then run them one after another.
*/


// splits a string into a vector of strings delimited by white space
std::vector<std::string> tokenizeString(std::string input)
{
	std::stringstream stringStream(input);
	std::istream_iterator<std::string> begin(stringStream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> stringVector(begin, end);
	return stringVector;
}


// main method
int main()
{
	// the string that holds the command
	std::string commandString = "";

	while (1)
	{
		// read the command fron cin
		std::cout << "batch-shell> ";
		getline(std::cin, commandString);

		// split the string into a vector of strings
		std::vector<std::string> tokenizedStrings = tokenizeString(commandString);
		for(int i = 0; i < tokenizedStrings.size(); ++i)
		{
			// exit the program if given the command
			if (tokenizedStrings[i] == "exit")
			{
				return 0;
			}

			// fork the program and call exec on the current string token
			try
			{
				pid_t pid = fork();

				// child process
				if (pid == 0)
				{
					std::cout << "Child process id: " << getpid() << std::endl;
					char args[0];
					execlp(tokenizedStrings[i].c_str(), args);
				}
				// parent process waits for the child to finish before continuing
				else
				{
					int *status;
					waitpid(pid, status, 0);
				}
			}
			catch (int exception)
			{
			}
		}
	}

	return 0;
}
