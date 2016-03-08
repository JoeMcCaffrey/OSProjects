/*

Joseph McCaffrey

Lab 1 Part 2
Process termination

*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<errno.h>

int main(){

 int proc;
 int killrc;

 printf("Enter the process id to kill: \n");
 scanf("%d", &proc);
 
// if( proc < 1){
//  printf("Invalid input, you cannot kill all processes\n");
//  return -1;
// }
 
 killrc = kill(proc,SIGKILL);

 if(errno == EPERM){
  
  printf("Error you don't have permission to kill that process id \n");
  return -1 ;
 }
 else if( errno == EINVAL){
  printf("Error invalid sig argument \n");
  return -1;
 }
 else if(errno == ESRCH){
  printf("Error no process found for that id \n");
  return -1 ;
 }
 else{
 

 printf("kill successful\n");
 return 0;
 }

}

