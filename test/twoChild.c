

#include<stdio.h>
#include<stdlib.h>


int main(){



 int pid  = fork();


 if(pid ==0){


  printf("im child and my pid is %d\n", getpid());

 }
 else{
  printf("im the parent at %d\n",getpid());
  int pid2 = fork();

  if (pid2 == 0){

   printf("Im the second child and my pid is %d\n",getpid());

  }
  else{

  printf("Im the parent at %d\n",getpid());

  }
 }

}
