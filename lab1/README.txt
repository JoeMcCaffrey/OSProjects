Operating Systems Lab 1 README/Design Document
James Calabro
Joe McCaffrey


Running Instructions:
Process Creation - Execute ./processCreation.out
Process Termination - Execure ./processTermination.out
Batch Scheduler - Execute ./batchScheduler.out

Process Creation:
The general structure of this program is that there is a nested if statement and each one deals with the creation of a child process and waiting for a process to finish. There isn't much else to talk about.

Process Termination:
This program reads in a process id from the standard in and tries to terminate it. If an error such as being unable to find a process with the entered pid occurs, it says so then exits. If the process was able to be terminated, it says so and exits. If the process was found but was unable to be terminated (i.e. the user does not have permission to close it), it says so and exits.

Batch Scheduler:
Once this program is launched, it reads commands to be executed from the batch-scheduler command prompt. It then takes all those programs, forks for each one, then execs on each program. The batch-scheduler then prompts you again for the next patch of processes to be executed. Type 'exit' into the batch-scheduler command prompt to quit the program.
