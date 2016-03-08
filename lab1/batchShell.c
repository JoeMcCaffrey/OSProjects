

#include<stdio.h>
#include<stdlib.h>




int main(){

 int i;
 char * command;
 char * token;

 while(1){

  printf("batch-shell> ");
  scanf("%s",command);
  
  

  while((token = strsep(&command, " ")) != NULL){

  int child_pid = fork();
  


  if(child_pid == 0 ){
    printf("My procesess id is %d\n", getpid());
    execv("", token);
    

  }
  else{


  wait(child_pid);
  return 0;

  }
 }

 }

}
