#include <stdio.h>
#include <stdlib.h>

/*
Joseph McCaffrey 27434201
Emma Hyde 26802145

cs 377 discussion 3

This program forks a child process and displays the pid of itself 
The parent also prints the pid of itself

written and compiled on the elnux machines

*/

int main(){


 int pid = fork();

 if(pid ==0){

  printf("I'm the child and my pid is %d \n",getpid());
 }
 else{
  printf("I'm the parent and my pid is %d\n",getpid());
 }
 

 return 0;
}

