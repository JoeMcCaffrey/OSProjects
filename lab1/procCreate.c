/*

Joseph McCaffrey

Lab 1 part 1 Process Creation 


*/
#include<stdio.h>
#include<stdlib.h>

int main(){

 int pid = fork();
 int status;

 if(pid ==0){
  printf("I'm the child and my pid is: %d \n",getpid());
  int pid2 = fork();
  if(pid2 == 0 ){
    printf("I'm the grandchild and my pid is: %d\n",getpid());
    sleep(10);
    printf("Grandchild with process %d has completed\n",getpid());
  }
  else{
   waitpid(-1,&status,0);
   printf("Child with process %d has completed\n",getpid());
  }
 }
 else{
   printf("Parent process and my pid is %d\n",getpid());
   waitpid(-1,&status,0);
   printf("Parent with process %d has completed\n",getpid());
 }

}



