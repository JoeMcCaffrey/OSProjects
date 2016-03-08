/***************************************

* Student 1 :  Joseph McCaffrey  Student1 ID: 27434201

* Student 2 : _______________ Student2 ID: ________

******************************************/


/* Discussion 1 exercise 

Joseph McCaffrey

This program takes a file and arranges its words in alphabetical order

This file takes a command line input for the file to read the
words from.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 100

int num_of_words;


// Read the text from file and store it in a buffer
int read_text(FILE *filename, char * buffer, int size_of_buffer){

 fread(buffer, size_of_buffer+1 , 1, filename);
 
// right now the memory buffer is a buffer of individual chars
// we want to turn that into an array of words
 
 fclose(filename);

 return 0;

}

// Get the next word from the buffer
void put_into_array(char * buffer, char * each_word[] ){

 // define the seperator
 const char s[2] = " ";
 
 char * token; 
 
// get the words seperated by spaces
 token = strtok(buffer, s); 
 
 int i =0 ;
 // put the words into the array
 while( token != NULL){
 
  each_word[i] = token;
  printf(" put into array  %s\n",token);
  token = strtok(NULL, s);
  i ++;
 }

// global variable keeping track of the number of words
 num_of_words = i ;
}


// create the list and return the alphabetically sorted output
int main (int argc, char * argv[]) {

 int file_size =0;
 
 FILE * file;

 //file = fopen("example.txt", "r");
 file = fopen(argv[1], "r");


 // find the size of the file
 
// send the pointer to the end of file and send it back
 fseek(file,0,2);

 file_size = ftell(file);

 printf("file size %d\n",file_size);
 fseek(file,0,SEEK_SET);
 
// create the buffer and the array to hold words

 char buffer[file_size];
 char * array[file_size];

// call to read in the words 
 read_text(file, buffer, file_size);

// call to put into array
 put_into_array(buffer, array);

// print the unsorted words
 int i,j ;
 printf("num of words %d\n", num_of_words);
 printf("-------------------------------\n");
 printf("Strings not in order:\n ");
 for (i =0; i<num_of_words; i++){
  printf("%s\n", array[i]);
 }

// bubble sorting

 for(i =1; i < num_of_words ; i++){
  for (j = 1; j < num_of_words; j++){
    
    if(strcasecmp(array[j-1], array[j]) >0){
   
     char * t  = array[j-1]; 
     array[j-1] = array[j];
     array[j] =t ;

    }
  }
 }

// print the sorted words
 printf("-----------------------------\n");
 printf("Strings in order:\n ");
 for (i =0; i<num_of_words; i++){
  printf("%s\n", array[i]);
 }

 return 0 ;

}
