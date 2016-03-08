/***************************************

* Student 1 :  ______________ Student1 ID: ________

* Student 2 : _______________ Student2 ID: ________

******************************************/


/* Discussion 1 exercise 

Joseph McCaffrey

This program takes a file and arranges its words in alphabetical order

*/




#include <stdio.h>
#include <string.h>

//#include <fstream>
//#include <iostream>
//#include <string>

#define SIZE 100

using namespace std;

struct node {

// data structure to hold the word

char x [SIZE];
struct node * next;

};



// Read the text from file and store it in a buffer
bool read_text(char * filename, void *buffer, int size_of_buffer){


//  char * word[100] ;
  string STRING;
  

  do{
    
     getline(filename,STRING);
     buffer << STRING;
//   buffer[count] = word;
   
 }  while(!filename.eof());


 return true;

}

// Get the next word from the buffer
bool next_word(void *buffer, void *word);

// Insert the word in the linked list
void insert(void *word);

// Note: You can use a helper function to insert. 
// The main purpose of the function is to find the location in existing linked list 
// to insert the new word.

// create the list and return the alphabetically sorted output

int main () {

 char * fp ;
 int file_size =0;
 

// fp = fopen(argv[1], "r");

// open the file using c++
 cout << "Enter the filename: ";
 string file;
 cin >> file;

// ifstream file(file_c_str());
 ifstream infile;
 infile.open(file);
 
 if(!infile.is_open())
  printf("opening of file failed \n");


// create the head node of the linked list

 struct node * head;

 head = (struct node *) malloc (sizeof(struct node));


// find the size of the file
 
 //fseek(fp,0,2);

// file_size = ftell(fp);

 // make a buffer of the size of the file
 
 //void * buffer = malloc(file_size);

 fp = infile;
 read_text(fp , buffer, file_size);
 
 


// call read_text to put the first word into the linked list


 return 0 ;

}
