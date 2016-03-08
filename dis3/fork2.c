#include<stdio.h>
#include<stdlib.h>
/*

Joseph McCaffrey 27434201
Emma Hyde 26802145

cs 377 Discussion 3

This program forks a child process, the child runs the ls command
The parent waits for the child to exit and then it itself exits

Written and compiled on the elnux machines

*/


int main(){


 int pid = fork();
 int status;
 char command[50];
 strcpy(command, "ls");
 if(pid ==0){
  printf("I'm the child and my pid is %d \n",getpid());
  printf("I'm the child and about to run the ls command\n");
  system(command);
  printf("I'm the child and about to exit\n");
  exit(0);
 }
 else{
 printf("I'm the parent and my pid is %d\n",getpid());
 printf("I'm the parent and I'm going to wait for the child\n");
 waitpid(-1,&status,0);
 printf("I'm the parent, the child exited so im going to exit\n");

 }

 return 0;
}
